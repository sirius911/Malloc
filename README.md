# Malloc
his project is about implementing a dynamic memory allocation mechanism.

### Tuto
[Master Memory Management: Create Your Own malloc Library from Scratch](https://medium.com/a-42-journey/how-to-create-your-own-malloc-library-b86fedd39b96)

Traduction : 

Êtes-vous curieux de savoir comment fonctionne la mémoire allouée dynamiquement ? Je l'étais aussi, et j'ai donc décidé de construire ma propre implémentation de malloc.

Dans cet article, je vais partager tout ce que vous devez savoir sur *malloc* - pourquoi il existe, comment il fonctionne, et comment le construire vous-même en utilisant les fonctions mmap/munmap et les algorithmes de gestion de la mémoire. J'ai même inclus mon projet terminé sur [GitHub](https://github.com/jterrazz/42-malloc) pour que vous puissiez vous y référer. Rejoignez-moi dans l'apprentissage de la gestion de la mémoire en construisant votre propre bibliothèque malloc ! 👷

```c
// nous allons implémenter ces méthodes
void malloc(size_t size);
void free(void ptr);
void realloc(void ptr, size_t size);
void calloc(size_t count, size_t size);

// Autorise les appels à demander de la mémoire
#include <sys/mman.h>

void mmap(void addr, size_t len, int prot, int flags, int fd, off_t offset);
int munmap(void addr, size_t len);

// Appels autorisés pour limiter nos appels de mémoire
#include <sys/resource.h>

int getrlimit(int resource, struct rlimit rlp);
int strlimit(int resource, const struct rlimit rlp);
```

## La gestion de la mémoire
Le langage C gère les variables en mémoire, mais il y a certaines limites à cela.

Les variables **statiques** et **globales** sont stockées dans la mémoire principale avec le code exécutable du programme et y restent pendant toute la durée du programme.

Les variables à **durée automatique**, comme une variable *int* locale dans une fonction, sont stockées sur la pile et sont créées et détruites chaque fois qu'une fonction est appelée ou retournée.

Cela peut être problématique pour deux raisons :
- Parce que la taille de l'allocation doit être fixée au moment de la compilation.Parce que la durée de vie d'une variable ne peut pas être ajustée 
- il n'est pas possible de choisir quand les créer ou les supprimer.

### L'allocation dynamique et *mmap*

```c
#include <sys/mman.h>

void mmap(void addr, size_t len, int prot, int flags, int fd, off_t offset);
```
Heureusement, le **noyau UNIX** fournit un appel système (**system call**) qui nous permet d'allouer de la mémoire avec une taille spécifiée. Dans cet article, nous utiliserons la fonction *mmap()* pour faire correspondre la **mémoire physique aux adresses virtuelles** et obtenir un pointeur sur le début, mais vous pouvez également utiliser _sbrk_. mmap est un excellent outil pour gérer la mémoire et les adresses virtuelles.

#### Voici malloc

Si *mmap* peut renvoyer une zone de mémoire, pourquoi avons-nous besoin de *malloc* ? Les appels au système prennent du temps et la plupart des programmes demandent et libèrent fréquemment une grande quantité de mémoire. Faire un appel système à chaque fois réduirait considérablement les performances.

Malloc aide à résoudre ce problème en agissant comme une enveloppe de performance (**performance wrapper**) autour de *mmap*. Nous pouvons préallouer plus de mémoire que nécessaire pour éviter les appels système, au prix d'une petite surcharge de mémoire. Les nouveaux appels à Malloc utiliseront l'espace préalloué jusqu'à ce que l'on ait besoin de plus d'espace.

## Mise en oeuvre

### La librairie

La bibliothèque *malloc* alloue un bloc de mémoire et renvoie un pointeur à son début. Lorsque le programme a fini d'utiliser la mémoire, le pointeur est transmis à *free()* pour la libérer. *realloc* augmente ou diminue la taille d'un pointeur *malloc* et préserve ses données, en renvoyant un nouveau pointeur.

### La Structure des données

Une zone de mémoire allouée par *mmap* s'appelle un tas(**heap**). Le tas est rempli de blocs (**blocks**). Les *heaps* et les *blocks* ont tous deux des métadonnées à leur début. Voici la structure d'un tas avec un bloc (*résultant d'un seul appel à malloc*).
![structure_heap](https://github.com/sirius911/Malloc/assets/25301163/e8ef7195-8c51-4278-84a9-d141a20c815f)

Nous pouvons utiliser les structures de métadonnées suivantes :
```c
typedef struct s_heap {
    struct s_heap   *prev;
    struct s_heap   *next;
    t_heap_group    group;
    size_t          total_size;
    size_t          free_size;
    size_t          block_count;
} t_heap;

typdef struct s_block {
    struct s_block  *prev;
    struct s_block  *next;
    size_t          data_size;
    bool            freed;
} t_block;
```
En *enchaînant* les blocs avec les pointeurs *next* et *prev*, nous pouvons parcourir le tas et accéder à n'importe quel bloc. Les structures C ont toujours une taille fixe, que nous pouvons utiliser pour passer en toute sécurité d'un pointeur de métadonnées au début des données. Cela nous aide à gérer efficacement le tas et les blocs.
```c
#define HEAP_START(start) ((void *)start + sizeof(t_heap))
#define BLOCK_SHIFT(start) ((void *)start + sizeof(t_block))
```
### Performance

Pour **préallouer efficacement la mémoire**, nous classons les blocs en trois catégories de taille : *SMALL*, *TINY* et *LARGE*. En règle générale, au moins 100 blocs SMALL et TINY doivent tenir dans leur propre tas. Les blocs plus grands, en revanche, ne sont pas préalloués.

La fonction *getpagesize()* renvoie le nombre d'octets d'une page mémoire, qui est un bloc de longueur fixe utilisé pour l'allocation de mémoire et le mappage de fichiers par mmap(2).

Il est plus efficace d'utiliser un multiple de *getpagesize()* pour définir la taille d'un tas. Par exemple, sur mon système, la taille de la page est de 4096 octets (lancez *getconf PAGE_SIZE*).

Supposons que nous utilisions 128 octets comme taille maximale pour les petits blocs. Si nous remplissons un tas avec 128 d'entre eux, cela nous donne une TINY_HEAP_ALLOCATION_SIZE de 16KB (128 * 128). Cependant, comme chaque malloc doit stocker ses métadonnées (sizeof(t_block) = 32 octets), nous ne pourrons pas contenir les 128 blocs dans le tas. 16 Ko / (128 + 32) = 102,4, nous pouvons donc placer **102 mallocs** de **128 octets** dans le tas de **16 Ko**, y compris le *t_heap* au début du tas.

Prenons les valeurs suivantes :
```c
#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAPALLOCATION_SIZE / 128)
#define SMALL_HEAP_ALLOCATION_SIZE (16 * getpagesize())
#define SMALL_BLOCK_SIZE (SMALL_HEAPèALLOCATION_SIZE / 128)
```
### L'algorithme de *malloc*

Lorsque *malloc* est appelé, il recherche un espace disponible et renvoie son adresse. Cependant, comme les adresses virtuelles des anciens blocs ne peuvent pas être modifiées, il faut gérer l'espace libre qui peut être fragmenté entre eux. Pour cela, il faut :
1. *Malloc* recherche une référence à la liste du tas (si elle existe) et la stocke dans une variable globale.
2. Il parcourt chaque tas pour essayer de **remplir un espace libéré** en utilisant la stratégie du premier ajustement(**first fit strategy**) - il parcourt chaque bloc du début à la fin jusqu'à ce qu'il trouve un bloc libéré avec suffisamment d'espace.
3. Si aucun bloc approprié n'est trouvé, *malloc* ajoute un nouveau bloc à la fin du dernier tas.
4. Si le dernier tas n'a pas assez d'espace disponible, l'algorithme crée un nouveau tas en appelant *mmap*.

```c
void *heap = (t_heap *)mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1,0);
```
### Free et le problème de fragmentation

![defrag_free](https://github.com/sirius911/Malloc/assets/25301163/2cfc5c3b-2cea-41cf-9cd1-93f22f5b67d8)

Lorsque la fonction *free* est appelée, le programme définit le statut du bloc demandé comme étant libéré. Cependant, comme les adresses virtuelles des blocs ne peuvent pas être modifiées (**freed**), nous nous retrouvons souvent avec une mémoire libérée fragmentée dans le tas(**heap**) (*comme le montre le graphique ci-dessus*).

1. **SI** block->prev->freed || block->next->freed, nous les fusionnons.
2. **Si** il s'agit du dernier bloc du tas, nous *munmap* pour libérer de la mémoire au système (mais nous devons toujours garder un bloc préalloué).
```c
nummap(heap, heap->total_size);
```
### Realloc

*Realloc* revient essentiellement à appeler *malloc* + *memcpy* + *free*.

Si l'espace demandé est nul, le comportement est défini par l'implémentation. J'ai choisi d'implémenter une version "paresseuse" qui renvoie simplement ptr. Cependant, il est important de noter que *realloc(ptr, 0)* ne doit pas être utilisé en remplacement de free.

### Tester

La meilleure façon de tester notre *malloc* est de l'utiliser dans des commandes habituelles, comme ls ou vim. Créez le fichier run.sh suivant et exécutez-le avant la commande désirée *sh run.sh ${CMD}*.

```shell
#!/bin/sh
export DYLD_LIBRARY_PATH=.
export DYLD_INSERT_LIBRARIES=libft_malloc.so
export DYLD_FORCE_FLAT_NAMESPACE=1
$@
```
### Aligner la mémoire sous MacOS

J'ai eu du mal à comprendre pourquoi certaines commandes, comme vim, provoquaient une erreur de segmentation (**segmentation fault**) sur mon système. Il s'avère que le malloc de **MacOS** aligne les données sur **16 octets**. Pour mettre cela en œuvre, il suffit de définir *size = (size + 15) & ~15*.

J'espère que cela vous a aidé à mieux comprendre la bibliothèque malloc. Si vous avez besoin de plus d'informations, n'hésitez pas à consulter l'implémentation terminée sur mon [GitHub](https://github.com/jterrazz/42-malloc). Je serai également heureux de répondre à vos questions ou commentaires.😁


