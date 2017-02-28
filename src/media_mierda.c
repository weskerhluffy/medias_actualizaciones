/*
 * media_mierda.c
 *
 *  Created on: 02/03/2016
 *      Author: ernesto
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#define CACA_COMUN_TAM_MAX_LINEA 12
#define CACA_LOG_MAX_TAM_CADENA 200

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

typedef unsigned int natural;
typedef long long tipo_dato;

typedef long long bitch_vector;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){printf("fuck\n");sleep(10);}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

void caca_log_debug_func(const char *format, ...);

#define MEDIA_MIERDA_MAX_OPERACIONES 100000
#define MEDIA_MIERDA_MAX_ELEMENTOS_INPUT MEDIA_MIERDA_MAX_OPERACIONES
#define MEDIA_MIERDA_MAX_VALOR INT_MAX
#define MEDIA_MIERDA_VALOR_INVALIDO -1LL

#if 1

#define AVL_TREE_VALOR_INVALIDO MEDIA_MIERDA_VALOR_INVALIDO

struct avl_tree_node_s {
	tipo_dato llave;
	natural altura;
	natural num_decendientes;
	natural indice_en_arreglo;
	natural ocurrencias;
	tipo_dato pasajero_oscuro;
	struct avl_tree_node_s *left;
	struct avl_tree_node_s *right;
	struct avl_tree_node_s *padre;
};

typedef struct avl_tree_node_s avl_tree_node_t;

struct avl_tree_s {
	natural max_nodos;
	natural nodos_realmente_en_arbol;
	natural nodos_usados;
	struct avl_tree_node_s *root;
	avl_tree_node_t *nodos_mem;
	natural *nodos_libres_idx;
	unsigned long siguiente_idx_para_usar;
	unsigned long ultimo_idx_anadido;
};

typedef struct avl_tree_s avl_tree_t;

typedef struct avl_tree_iterator_t {
	avl_tree_t *arbolin;
	char *contador_visitas;
	avl_tree_node_t *nodo_actual;
} avl_tree_iterator_t;

/* Create a new AVL tree. */
avl_tree_t *avl_tree_create(avl_tree_t **arbolin, int max_nodos) {
	avl_tree_t *tree = NULL;

	assert_timeout(arbolin);

	tree = calloc(1, sizeof(avl_tree_t));

	assert_timeout(tree);

	tree->max_nodos = max_nodos;

	tree->nodos_mem = calloc(max_nodos * 2, sizeof(avl_tree_node_t));

	assert_timeout(tree->nodos_mem);

	tree->root = NULL;

	*arbolin = tree;

	tree->nodos_libres_idx = calloc(max_nodos, sizeof(natural));
	memset(tree->nodos_libres_idx, 0xffff, sizeof(natural) * max_nodos);

	assert_timeout(tree->nodos_libres_idx);

	return tree;
}

static inline void avl_tree_destroy(avl_tree_t *arbolin) {
	free(arbolin->nodos_mem);
	free(arbolin->nodos_libres_idx);
	free(arbolin);

}

/* Initialize a new node. */
avl_tree_node_t *avl_tree_create_node(avl_tree_t *arbolin) {
	avl_tree_node_t *node = NULL;

	assert_timeout(
			arbolin->siguiente_idx_para_usar < arbolin->ultimo_idx_anadido
					|| ((arbolin->siguiente_idx_para_usar
							== arbolin->ultimo_idx_anadido)
							&& arbolin->nodos_usados < arbolin->max_nodos));

	if (arbolin->siguiente_idx_para_usar < arbolin->ultimo_idx_anadido) {
		node = arbolin->nodos_mem
				+ arbolin->nodos_libres_idx[arbolin->siguiente_idx_para_usar
						% arbolin->max_nodos];
		node->indice_en_arreglo =
				arbolin->nodos_libres_idx[arbolin->siguiente_idx_para_usar
						% arbolin->max_nodos];

		arbolin->nodos_libres_idx[arbolin->siguiente_idx_para_usar
				% arbolin->max_nodos] = 0xffffffff;
		arbolin->siguiente_idx_para_usar++;
	} else {
		node = arbolin->nodos_mem + arbolin->nodos_usados++;
		node->indice_en_arreglo = arbolin->nodos_usados - 1;
	}
	arbolin->nodos_realmente_en_arbol++;
	caca_log_debug("aumentando nodos realmente en arbol a %u",
			arbolin->nodos_realmente_en_arbol);
	return node;
}

/* Find the height of an AVL no repulsivamente */
int avl_tree_node_height(avl_tree_node_t *node) {
	return node ? node->altura : 0;
}

static inline void avl_tree_node_actualizar_altura(avl_tree_node_t *node) {
	int height_left = 0;
	int height_right = 0;

	if (node->left) {
		height_left = node->left->altura;
	}
	if (node->right) {
		height_right = node->right->altura;
	}
	if (node->left || node->right) {
		node->altura = (height_right > height_left ? height_right : height_left)
				+ 1;
	} else {
		node->altura = 0;
	}
}

static inline void avl_tree_node_actualizar_num_decendientes(
		avl_tree_node_t *node) {
	int conteo_left = 0;
	int conteo_right = 0;

	if (node->left) {
		conteo_left = node->left->num_decendientes;
	}
	if (node->right) {
		conteo_right = node->right->num_decendientes;
	}
	if (node->left || node->right) {
		node->num_decendientes = conteo_left + conteo_right
				+ (node->left ? 1 : 0) + (node->right ? 1 : 0);
	} else {
		node->num_decendientes = 0;
	}
}

/* Left Left Rotate */
avl_tree_node_t *avl_tree_rotate_leftleft(avl_tree_node_t *node) {
	avl_tree_node_t *a = node;
	avl_tree_node_t *b = a->left;
	avl_tree_node_t *padre = NULL;

	padre = node->padre;

	a->left = b->right;
	b->right = a;

	avl_tree_node_actualizar_altura(a);
	avl_tree_node_actualizar_altura(b);
	avl_tree_node_actualizar_num_decendientes(a);
	avl_tree_node_actualizar_num_decendientes(b);

	a->padre = b;
	b->padre = padre;
	if (a->left) {
		a->left->padre = a;
	}

	return (b);
}

/* Left Right Rotate */
avl_tree_node_t *avl_tree_rotate_leftright(avl_tree_node_t *node) {
	avl_tree_node_t *a = node;
	avl_tree_node_t *b = a->left;
	avl_tree_node_t *c = b->right;
	avl_tree_node_t *padre = NULL;

	padre = node->padre;

	a->left = c->right;
	b->right = c->left;
	c->left = b;
	c->right = a;

	avl_tree_node_actualizar_altura(a);
	avl_tree_node_actualizar_altura(b);
	avl_tree_node_actualizar_altura(c);

	avl_tree_node_actualizar_num_decendientes(a);
	avl_tree_node_actualizar_num_decendientes(b);
	avl_tree_node_actualizar_num_decendientes(c);

	a->padre = c;
	b->padre = c;
	c->padre = padre;
	if (a->left) {
		a->left->padre = a;
	}
	if (b->right) {
		b->right->padre = b;
	}

	return (c);
}

/* Right Left Rotate */
avl_tree_node_t *avl_tree_rotate_rightleft(avl_tree_node_t *node) {
	avl_tree_node_t *a = node;
	avl_tree_node_t *b = a->right;
	avl_tree_node_t *c = b->left;
	avl_tree_node_t *padre = NULL;

	padre = node->padre;

	a->right = c->left;
	b->left = c->right;
	c->right = b;
	c->left = a;

	avl_tree_node_actualizar_altura(a);
	avl_tree_node_actualizar_altura(b);
	avl_tree_node_actualizar_altura(c);

	avl_tree_node_actualizar_num_decendientes(a);
	avl_tree_node_actualizar_num_decendientes(b);
	avl_tree_node_actualizar_num_decendientes(c);

	a->padre = c;
	b->padre = c;
	c->padre = padre;
	if (a->right) {
		a->right->padre = a;
	}
	if (b->left) {
		b->left->padre = b;
	}

	return (c);
}

/* Right Right Rotate */
avl_tree_node_t *avl_tree_rotate_rightright(avl_tree_node_t *node) {
	avl_tree_node_t *a = node;
	avl_tree_node_t *b = a->right;
	avl_tree_node_t *padre = NULL;

	padre = node->padre;

	a->right = b->left;
	b->left = a;

	avl_tree_node_actualizar_altura(a);
	avl_tree_node_actualizar_altura(b);

	avl_tree_node_actualizar_num_decendientes(a);
	avl_tree_node_actualizar_num_decendientes(b);

	a->padre = b;
	b->padre = padre;
	if (a->right) {
		a->right->padre = a;
	}

	return (b);
}

/* Find the balance of an AVL node */
int avl_tree_balance_factor(avl_tree_node_t *node) {
	int bf = 0;

	if (node->left)
		bf += avl_tree_node_height(node->left);
	if (node->right)
		bf -= avl_tree_node_height(node->right);

	return bf;
}

static inline avl_tree_node_t *avl_tree_balance_node_insertar(
		const avl_tree_node_t *node, const tipo_dato llave_nueva,
		const tipo_dato pasajero_oscuro) {
	avl_tree_node_t *newroot = NULL;
	avl_tree_node_t *nodo_actual = NULL;

	newroot = (avl_tree_node_t *) node;
	nodo_actual = node->padre;
	while (nodo_actual) {

		int bf = 0;
		avl_tree_node_t *padre = NULL;
		avl_tree_node_t **rama_padre = NULL;

		bf = avl_tree_balance_factor(nodo_actual);

		if (bf >= 2) {
			/* Left Heavy */
			if (llave_nueva > nodo_actual->left->llave) {
				newroot = avl_tree_rotate_leftright(nodo_actual);
			} else {
				if (llave_nueva < nodo_actual->left->llave) {
					newroot = avl_tree_rotate_leftleft(nodo_actual);
				} else {
					if (pasajero_oscuro > nodo_actual->left->pasajero_oscuro) {
						newroot = avl_tree_rotate_leftright(nodo_actual);
					} else {
						newroot = avl_tree_rotate_leftleft(nodo_actual);
					}
				}
			}

		} else if (bf <= -2) {
			/* Right Heavy */
			if (llave_nueva < nodo_actual->right->llave) {
				newroot = avl_tree_rotate_rightleft(nodo_actual);
			} else {
				if (llave_nueva > nodo_actual->right->llave) {
					newroot = avl_tree_rotate_rightright(nodo_actual);
				} else {
					if (pasajero_oscuro < nodo_actual->right->pasajero_oscuro) {
						newroot = avl_tree_rotate_rightleft(nodo_actual);
					} else {
						newroot = avl_tree_rotate_rightright(nodo_actual);
					}
				}
			}

		} else {
			/* This node is balanced -- no change. */
			newroot = nodo_actual;
			avl_tree_node_actualizar_altura(nodo_actual);
		}

		if (newroot->padre) {
			padre = newroot->padre;
			if (llave_nueva < padre->llave) {
				rama_padre = &padre->left;
			} else {
				if (llave_nueva > padre->llave) {
					rama_padre = &padre->right;
				} else {
					if (pasajero_oscuro < padre->pasajero_oscuro) {
						rama_padre = &padre->left;
					} else {
						if (pasajero_oscuro > padre->pasajero_oscuro) {
							rama_padre = &padre->right;
						} else {
							assert_timeout(0);
						}
					}
				}
			}
			*rama_padre = newroot;
		}

		nodo_actual = nodo_actual->padre;
	}

	return (newroot);
}

/* Balance a given tree */
void avl_tree_balance_insertar(avl_tree_t *tree, avl_tree_node_t *nodo,
		tipo_dato llave_nueva, tipo_dato pasajero_oscuro) {

	avl_tree_node_t *newroot = NULL;

	newroot = avl_tree_balance_node_insertar(nodo, llave_nueva,
			pasajero_oscuro);

	if (newroot != tree->root) {
		tree->root = newroot;
	}
}

/* Insert a new node. */
void avl_tree_insert(avl_tree_t *tree, tipo_dato value,
		tipo_dato pasajero_oscuro) {
	avl_tree_node_t *node = NULL;
	avl_tree_node_t *next = NULL;
	avl_tree_node_t *last = NULL;

	/* Well, there must be a first case */
	if (tree->root == NULL) {
		node = avl_tree_create_node(tree);
		node->llave = value;
		node->pasajero_oscuro = pasajero_oscuro;

		tree->root = node;

		/* Okay.  We have a root already.  Where do we put this? */
	} else {
		next = tree->root;

		while (next != NULL) {
			last = next;

			next->num_decendientes++;

			if (value < next->llave) {
				next = next->left;
			} else {
				if (value > next->llave) {
					next = next->right;
				} else {
					if (value == next->llave) {

						if (pasajero_oscuro < next->pasajero_oscuro) {
							next = next->left;
						} else {
							if (pasajero_oscuro > next->pasajero_oscuro) {
								next = next->right;
							} else {
								avl_tree_node_t *ancestro_actal = NULL;
								/* Have we already inserted this node? */
								next->ocurrencias++;
								caca_log_debug(
										"llave ya existe, aumentando contador a carajo %u\n",
										next->ocurrencias);

								ancestro_actal = next;
								while (ancestro_actal) {
									caca_log_debug(
											"bajando decendientes de %llu",
											ancestro_actal->llave);
									ancestro_actal->num_decendientes--;
									ancestro_actal = ancestro_actal->padre;
								}
								return;
							}
						}
					} else {
						caca_log_debug("verga, no es maior menor ni igual\n");
						assert_timeout(0);
					}
				}
			}
		}

		node = avl_tree_create_node(tree);
		node->llave = value;
		node->pasajero_oscuro = pasajero_oscuro;

		if (value < last->llave) {
			last->left = node;
		} else {
			if (value > last->llave) {
				last->right = node;
			} else {
				if (pasajero_oscuro < last->pasajero_oscuro) {
					last->left = node;
				} else {
					last->right = node;
				}
			}
		}

		node->padre = last;

	}
	node->ocurrencias = 1;

	avl_tree_balance_insertar(tree, node, value, pasajero_oscuro);
}

/* Find the node containing a given value */
avl_tree_node_t *avl_tree_find(avl_tree_t *tree, tipo_dato value,
		tipo_dato pasajero_oscuro) {
	avl_tree_node_t *current = tree->root;

	while (current) {
		if (value > current->llave) {
			current = current->right;
		} else {
			if (value < current->llave) {
				current = current->left;
			} else {
				if (pasajero_oscuro != AVL_TREE_VALOR_INVALIDO) {
					if (pasajero_oscuro > current->pasajero_oscuro) {
						current = current->right;
					} else {
						if (pasajero_oscuro < current->pasajero_oscuro) {
							current = current->left;
						} else {
							break;
						}
					}
				} else {
					break;
				}
			}
		}
	}

	return current ? current->llave == value ? current : NULL :NULL;
}

/* Do a depth first traverse of a node. */
void avl_tree_traverse_node_dfs(avl_tree_node_t *node, int depth) {
	int i = 0;

	if (node->left)
		avl_tree_traverse_node_dfs(node->left, depth + 2);

	for (i = 0; i < depth; i++)
		putchar(' ');
	printf("%lld: %d\n", node->llave, avl_tree_balance_factor(node));

	if (node->right)
		avl_tree_traverse_node_dfs(node->right, depth + 2);
}

/* Do a depth first traverse of a tree. */
void avl_tree_traverse_dfs(avl_tree_t *tree) {
	avl_tree_traverse_node_dfs(tree->root, 0);
}

static inline void avl_tree_iterador_ini(avl_tree_t *arbolin,
		avl_tree_iterator_t *iter) {
	iter->contador_visitas = calloc(arbolin->nodos_realmente_en_arbol,
			sizeof(char));
	assert_timeout(iter->contador_visitas);
	iter->arbolin = arbolin;
}

static inline void avl_tree_iterador_fini(avl_tree_iterator_t *iter) {
	free(iter->contador_visitas);
}

static inline bool avl_tree_iterador_hay_siguiente(avl_tree_iterator_t *iter) {
	return !(iter->nodo_actual == iter->arbolin->root
			&& iter->contador_visitas[iter->arbolin->root->indice_en_arreglo]
					== 2);
}

static inline bool avl_tree_iterador_hay_anterior(avl_tree_iterator_t *iter) {
	bool resultado = falso;
	resultado =
			iter->nodo_actual->left
					&& !iter->contador_visitas[iter->nodo_actual->left->indice_en_arreglo];
	return resultado;
}

static inline avl_tree_node_t* avl_tree_iterador_siguiente(
		avl_tree_iterator_t *iter) {
	int contador_actual = 0;
	avl_tree_node_t *nodo = NULL;
	avl_tree_node_t *last_of_us = NULL;
	avl_tree_node_t *nodo_actual = NULL;

	if (!iter->nodo_actual) {
		nodo_actual = iter->nodo_actual = iter->arbolin->root;
	}

	if (!avl_tree_iterador_hay_siguiente(iter)) {
		return NULL;
	}

	contador_actual =
			iter->contador_visitas[iter->nodo_actual->indice_en_arreglo];

	iter->contador_visitas[iter->nodo_actual->indice_en_arreglo]++;

	switch (contador_actual) {
	case 0:
	case 1:
		if (!contador_actual) {
			nodo_actual = iter->nodo_actual->left;
			if (!nodo_actual) {
				return iter->nodo_actual;
			}
		} else {
			nodo_actual = iter->nodo_actual->right;
			if (!nodo_actual) {
				nodo_actual = iter->nodo_actual->padre;
				while (nodo_actual
						&& iter->contador_visitas[nodo_actual->indice_en_arreglo]
								== 2) {
					last_of_us = nodo_actual;
					nodo_actual = nodo_actual->padre;
				}
				if (!nodo_actual) {
					if (last_of_us) {
						iter->nodo_actual = last_of_us;
					}
				} else {
					iter->nodo_actual = nodo_actual;
				}
				return nodo_actual;
			}
		}

		while (nodo_actual) {
			last_of_us = nodo_actual;
			iter->contador_visitas[nodo_actual->indice_en_arreglo]++;
			nodo_actual = last_of_us->left;
		}

		nodo = iter->nodo_actual = last_of_us;

		break;
	default:
		assert_timeout(0)
		;
		break;
	}

	return nodo;
}

static inline avl_tree_node_t* avl_tree_iterador_anterior(
		avl_tree_iterator_t *iter) {
	int contador_actual = 0;
	avl_tree_node_t *nodo = NULL;
	avl_tree_node_t *last_of_us = NULL;
	avl_tree_node_t *nodo_actual = NULL;

	if (!iter->nodo_actual) {
		nodo_actual = iter->nodo_actual = iter->arbolin->root;
	}

	if (!avl_tree_iterador_hay_anterior(iter)) {
		return NULL;
	}

	contador_actual =
			iter->contador_visitas[iter->nodo_actual->indice_en_arreglo];

	iter->contador_visitas[iter->nodo_actual->indice_en_arreglo]++;

	switch (contador_actual) {
	case 0:
	case 1:
		if (!contador_actual) {
			nodo_actual = iter->nodo_actual->left;
			if (!nodo_actual) {
				return iter->nodo_actual;
			}
		} else {
			nodo_actual = iter->nodo_actual->right;
			if (!nodo_actual) {
				nodo_actual = iter->nodo_actual->padre;
				while (nodo_actual
						&& iter->contador_visitas[nodo_actual->indice_en_arreglo]
								== 2) {
					last_of_us = nodo_actual;
					nodo_actual = nodo_actual->padre;
				}
				if (!nodo_actual) {
					if (last_of_us) {
						iter->nodo_actual = last_of_us;
					}
				} else {
					iter->nodo_actual = nodo_actual;
				}
				return nodo_actual;
			}
		}

		while (nodo_actual) {
			last_of_us = nodo_actual;
			iter->contador_visitas[nodo_actual->indice_en_arreglo]++;
			nodo_actual = last_of_us->left;
		}

		nodo = iter->nodo_actual = last_of_us;

		break;
	default:
		assert_timeout(0)
		;
		break;
	}

	return nodo;
}

static inline avl_tree_node_t* avl_tree_iterador_obtener_actual(
		avl_tree_iterator_t *iter) {
	avl_tree_node_t *nodo = NULL;

	if (!iter->nodo_actual) {
		avl_tree_iterador_siguiente(iter);
	}
	nodo = iter->nodo_actual;

	return nodo;

}

static inline char *avl_tree_inoder_node_travesti(avl_tree_node_t *nodo,
		char *buf, int profundidad_maxima) {
	char num_buf[100] = { '\0' };
	int profundidad = 0;
	int i = 0;

	assert_timeout(profundidad_maxima == -1 || profundidad != -1);

	if (nodo != NULL) {
		profundidad = profundidad_maxima - nodo->altura;

		assert_timeout(!nodo->right || nodo->right->padre == nodo);
		avl_tree_inoder_node_travesti(nodo->right, buf, profundidad_maxima);

		if (profundidad_maxima != -1) {
			for (i = 0; i < profundidad; i++) {
				strcat(buf, " ");
			}
		}
		sprintf(num_buf, "%lld", nodo->llave);
		strcat(buf, num_buf);
		if (profundidad_maxima != -1) {
			strcat(buf, "\n");
		}

		assert_timeout(!nodo->left || nodo->left->padre == nodo);
		avl_tree_inoder_node_travesti(nodo->left, buf, profundidad_maxima);

		/*
		 if (profundidad_maxima != -1) {
		 strcat(buf, "\n");
		 for (int i = 0; i <= profundidad; i++) {
		 strcat(buf, " ");
		 }
		 }
		 */
	}
	return buf;
}

static inline char* avl_tree_sprint(avl_tree_t *arbolini, char *buf) {
	avl_tree_inoder_node_travesti(arbolini->root, buf, -1);
	return buf;
}

static inline char *avl_tree_inoder_node_travesti_conteo(avl_tree_node_t *nodo,
		char *buf, int profundidad_maxima) {
	char num_buf[100] = { '\0' };
	int profundidad = 0;
	int i = 0;

	assert_timeout(profundidad_maxima == -1 || profundidad != -1);

	if (nodo != NULL) {
		profundidad = profundidad_maxima - nodo->altura;

		assert_timeout(!nodo->right || nodo->right->padre == nodo);
		avl_tree_inoder_node_travesti_conteo(nodo->right, buf,
				profundidad_maxima);

		if (profundidad_maxima != -1) {
			for (i = 0; i < profundidad; i++) {
				strcat(buf, " ");
			}
		}
		sprintf(num_buf, "%lld [%u,%u] (%u) ocu %u", nodo->llave,
				(natural) (nodo->llave >> 32), (natural) nodo->llave,
				nodo->num_decendientes, nodo->ocurrencias);
		strcat(buf, num_buf);
		if (profundidad_maxima != -1) {
			strcat(buf, "\n");
		}

		assert_timeout(!nodo->left || nodo->left->padre == nodo);
		avl_tree_inoder_node_travesti_conteo(nodo->left, buf,
				profundidad_maxima);

		/*
		 if (profundidad_maxima != -1) {
		 strcat(buf, "\n");
		 for (int i = 0; i <= profundidad; i++) {
		 strcat(buf, " ");
		 }
		 }
		 */
	}
	return buf;
}

static inline char* avl_tree_sprint_identado(avl_tree_t *arbolini, char *buf) {
	int profundidad_maxima = 0;

	*buf = '\0';

	if (!arbolini->root) {
		return buf;
	}

	profundidad_maxima = arbolini->root->altura;
	avl_tree_inoder_node_travesti_conteo(arbolini->root, buf,
			profundidad_maxima);
	return buf;
}

static inline avl_tree_node_t* avl_tree_iterador_asignar_actual(
		avl_tree_iterator_t *iter, tipo_dato llave) {
	avl_tree_t *arbolazo = NULL;
	avl_tree_node_t *nodo_actual = NULL;
	avl_tree_node_t *last_of_us = NULL;

	arbolazo = iter->arbolin;

	nodo_actual = arbolazo->root;

	while (nodo_actual) {
		last_of_us = nodo_actual;
		if (llave < nodo_actual->llave) {
			iter->contador_visitas[nodo_actual->indice_en_arreglo] = 1;
			nodo_actual = nodo_actual->left;
		} else {
			iter->contador_visitas[nodo_actual->indice_en_arreglo] = 2;
			if (llave > nodo_actual->llave) {
				nodo_actual = nodo_actual->right;
			} else {
				break;
			}

		}
	}

	if (nodo_actual) {
		iter->nodo_actual = nodo_actual;
	} else {
		iter->nodo_actual = last_of_us;
	}
	iter->contador_visitas[iter->nodo_actual->indice_en_arreglo] = 1;
	return iter->nodo_actual;
}

static inline avl_tree_node_t* avl_tree_max_min(avl_tree_t *arbolin, bool max) {
	avl_tree_node_t *nodo_actual = NULL;
	avl_tree_node_t *last_of_us = NULL;

	nodo_actual = last_of_us = arbolin->root;

	while (nodo_actual) {
		last_of_us = nodo_actual;
		if (max) {
			nodo_actual = nodo_actual->right;
		} else {
			nodo_actual = nodo_actual->left;
		}
	}

	return last_of_us;
}

static inline void avl_tree_validar_arbolin_indices(avl_tree_t *arbolin,
		avl_tree_node_t *nodo) {
	if (nodo != NULL) {
		assert_timeout(&arbolin->nodos_mem[nodo->indice_en_arreglo] == nodo);
		assert_timeout(!nodo->left || nodo->left->padre == nodo);
		avl_tree_validar_arbolin_indices(arbolin, nodo->left);
		assert_timeout(!nodo->right || nodo->right->padre == nodo);
		avl_tree_validar_arbolin_indices(arbolin, nodo->right);
	}
}

/* Balance a given node */
/* Given a non-empty binary search tree, return the node with minimum
 key value found in that tree. Note that the entire tree does not
 need to be searched. */
static inline avl_tree_node_t* avl_tree_siguiente_nodo_inorder(
		avl_tree_node_t *node) {
	avl_tree_node_t *current = node;

	/* loop down to find the leftmost leaf */
	while (current->left != NULL) {
		current = current->left;
	}

	return current;
}

static inline avl_tree_node_t *avl_tree_nodo_borrar(avl_tree_t *arbolini,
		avl_tree_node_t *root, tipo_dato key, bool ignora_conteo,
		tipo_dato pasajero_oscuro) {

	if (root == NULL) {
		return root;
	}

	if (key < root->llave) {
		root->left = avl_tree_nodo_borrar(arbolini, root->left, key,
				ignora_conteo, pasajero_oscuro);
		assert_timeout(!root->left || root->left->padre == root);
	} else {
		if (key > root->llave) {
			root->right = avl_tree_nodo_borrar(arbolini, root->right, key,
					ignora_conteo, pasajero_oscuro);
			assert_timeout(!root->right || root->right->padre == root);
		} else {
			if (pasajero_oscuro == AVL_TREE_VALOR_INVALIDO) {
				if ((root->ocurrencias - 1) == 0 || ignora_conteo) {
					if (root->left == NULL || root->right == NULL) {
						avl_tree_node_t *temp =
								root->left ? root->left : root->right;

						if (temp == NULL) {
							temp = root;
							root = NULL;
						} else {
							natural idx_en_arreglo = 0;
							avl_tree_node_t *padre = NULL;

							padre = root->padre;
							idx_en_arreglo = root->indice_en_arreglo;
							*root = *temp;
							root->padre = padre;
							root->indice_en_arreglo = idx_en_arreglo;
							if (root->left) {
								root->left->padre = root;
							}
							if (root->right) {
								root->right->padre = root;
							}
						}

						assert_timeout(
								arbolini->ultimo_idx_anadido
										- arbolini->siguiente_idx_para_usar
										< arbolini->max_nodos);
						arbolini->nodos_libres_idx[arbolini->ultimo_idx_anadido++
								% arbolini->max_nodos] =
								temp->indice_en_arreglo;
						memset(temp, 0, sizeof(avl_tree_node_t));
						temp->llave = AVL_TREE_VALOR_INVALIDO;
						arbolini->nodos_realmente_en_arbol--;
						caca_log_debug(
								"disminuiendo nodos realmente en arbol a %u",
								arbolini->nodos_realmente_en_arbol);

					} else {
						avl_tree_node_t *temp = avl_tree_siguiente_nodo_inorder(
								root->right);

						root->llave = temp->llave;
						root->ocurrencias = temp->ocurrencias;

						root->right = avl_tree_nodo_borrar(arbolini,
								root->right, temp->llave, verdadero,
								temp->pasajero_oscuro);
					}
				} else {
					root->ocurrencias--;
					return root;
				}
			} else {
				if (pasajero_oscuro < root->pasajero_oscuro) {
					root->left = avl_tree_nodo_borrar(arbolini, root->left, key,
							ignora_conteo, pasajero_oscuro);
					assert_timeout(!root->left || root->left->padre == root);
				} else {
					if (pasajero_oscuro > root->pasajero_oscuro) {
						root->right = avl_tree_nodo_borrar(arbolini,
								root->right, key, ignora_conteo,
								pasajero_oscuro);
						assert_timeout(
								!root->right || root->right->padre == root);
					} else {

						if ((root->ocurrencias - 1) == 0 || ignora_conteo) {
							if (root->left == NULL || root->right == NULL) {
								avl_tree_node_t *temp =
										root->left ? root->left : root->right;

								if (temp == NULL) {
									temp = root;
									root = NULL;
								} else {
									natural idx_en_arreglo = 0;
									avl_tree_node_t *padre = NULL;

									padre = root->padre;
									idx_en_arreglo = root->indice_en_arreglo;
									*root = *temp;
									root->padre = padre;
									root->indice_en_arreglo = idx_en_arreglo;
									if (root->left) {
										root->left->padre = root;
									}
									if (root->right) {
										root->right->padre = root;
									}
								}

								assert_timeout(
										arbolini->ultimo_idx_anadido
												- arbolini->siguiente_idx_para_usar
												< arbolini->max_nodos);
								arbolini->nodos_libres_idx[arbolini->ultimo_idx_anadido++
										% arbolini->max_nodos] =
										temp->indice_en_arreglo;
								memset(temp, 0, sizeof(avl_tree_node_t));
								temp->llave = AVL_TREE_VALOR_INVALIDO;
								arbolini->nodos_realmente_en_arbol--;
								caca_log_debug(
										"disminuiendo nodos realmente en arbol a %u",
										arbolini->nodos_realmente_en_arbol);

							} else {
								avl_tree_node_t *temp =
										avl_tree_siguiente_nodo_inorder(
												root->right);

								root->llave = temp->llave;
								root->ocurrencias = temp->ocurrencias;

								root->right = avl_tree_nodo_borrar(arbolini,
										root->right, temp->llave, verdadero,
										temp->pasajero_oscuro);
							}

						} else {
							root->ocurrencias--;
							return root;
						}
					}
				}
			}
		}
	}

	if (root == NULL) {
		return root;
	}

	avl_tree_node_actualizar_altura(root);
	avl_tree_node_actualizar_num_decendientes(root);

	int balance = avl_tree_balance_factor(root);

	if (balance > 1 && avl_tree_balance_factor(root->left) >= 0) {
		return avl_tree_rotate_leftleft(root);
	}

	if (balance > 1 && avl_tree_balance_factor(root->left) < 0) {
		return avl_tree_rotate_leftright(root);
	}

	if (balance < -1 && avl_tree_balance_factor(root->right) <= 0) {
		return avl_tree_rotate_rightright(root);
	}

	if (balance < -1 && avl_tree_balance_factor(root->right) > 0) {
		return avl_tree_rotate_rightleft(root);
	}

	return root;
}

void avl_tree_borrar(avl_tree_t *tree, tipo_dato value) {

	avl_tree_node_t *newroot = NULL;
	caca_log_debug("borrando valor %d", value);

	if (!tree->root) {
		return;
	}
	newroot = avl_tree_nodo_borrar(tree, tree->root, value, falso,
			AVL_TREE_VALOR_INVALIDO);

	if (newroot != tree->root) {
		tree->root = newroot;
	}
}

#endif

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifndef ONLINE_JUDGE
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
	struct timespec spec;
	char parte_milisecundos[50];

	ltime = time(NULL);

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

	caca_comun_current_utc_time(&spec);
#ifndef ONLINE_JUDGE
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
			"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
			+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = { '\0' };
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = { '\0' };
	char cadena_timestamp[100] = { '\0' };

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
}
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%2llu", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static inline int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix,
		int *num_filas, int *num_columnas, int num_max_filas,
		int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia\n");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			caca_log_debug("el numero raw %s\n", linea);
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			caca_log_debug("en col %d, fil %d, el valor %lu\n", indice_columnas,
					indice_filas, numero);
			indice_columnas++;
			caca_log_debug("las columnas son %d\n", indice_columnas);
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d\n", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

static inline tipo_dato media_mierda_core(avl_tree_t *arbolin, int numerin,
		natural idx, bool anadir) {
	bool se_hizo_algo = falso;
	caca_log_debug("%s el num %d(%u)", anadir?"poniendo":"quitando", numerin,
			idx);
	if (anadir) {
		avl_tree_insert(arbolin, numerin, idx);
		se_hizo_algo = verdadero;
	} else {
		if (arbolin->nodos_realmente_en_arbol > 1
				&& avl_tree_find(arbolin, numerin, AVL_TREE_VALOR_INVALIDO)) {
			avl_tree_borrar(arbolin, numerin);
			se_hizo_algo = verdadero;
		}
	}
	caca_log_debug("se izo algo %u %s", se_hizo_algo, se_hizo_algo?"si":"nel");
	return 0;
}

void media_mierda_main() {
	natural num_filas = 0;
	tipo_dato num_estrellas = 0;
	natural *numeros = NULL;
	natural bytes_read = 0;

	avl_tree_t *arbolin = NULL;
	int i = 0;

	char *buffer = NULL;
	buffer = calloc(CACA_COMUN_TAM_MAX_LINEA * 1000, sizeof(char));
	assert_timeout(buffer);

	numeros = calloc(MEDIA_MIERDA_MAX_ELEMENTOS_INPUT, sizeof(natural));
	assert_timeout(numeros);

	caca_comun_lee_matrix_long_stdin(&num_estrellas, (int*) &num_filas, NULL, 1,
			1);

	caca_log_debug("cuando veiamos estreshas %lu", num_estrellas);

	avl_tree_create(&arbolin, MEDIA_MIERDA_MAX_ELEMENTOS_INPUT);

	while ((bytes_read = getline(&buffer, &(size_t ) { 0 }, stdin)) != -1) {
		char ope = '\0';
		natural num_actual = 0;

		caca_log_debug("la cadenita leida %s", buffer);
		sscanf(buffer, "%c %d", &ope, &num_actual);

		caca_log_debug("la op %c, el num %d", ope, num_actual);

		assert_timeout(ope == 'a' || ope == 'r');

		media_mierda_core(arbolin, num_actual, i, ope == 'a');
		i++;
	}

	avl_tree_destroy(arbolin);

	/*

	 for (i = 0; i < num_estrellas; i++) {
	 tipo_dato num_estrellas_abajo_izq = 0;
	 tipo_dato estrella_negra = 0;

	 estrella_negra = *(coordenadas_enmascaradas + i);

	 caca_log_debug("insertando %u,%u (%lu)", (natural)(estrella_negra>>32),
	 (natural)estrella_negra, estrella_negra);

	 avl_tree_insert(arbolin, estrella_negra);
	 avl_tree_insert(arbolin, estrella_negra);

	 #ifndef ONLINE_JUDGE
	 avl_tree_validar_arbolin_indices(arbolin, arbolin->root);
	 memset(buffer, '\0', CACA_COMUN_TAM_MAX_LINEA * 10);
	 #endif

	 caca_log_debug("el arbolin aora es\n%s",
	 avl_tree_sprint_identado(arbolin,buffer));

	 #ifndef MEDIA_MIERDA_CONTEO_PROBADO
	 #ifndef MEDIA_MIERDA_CONTEO_ITERA

	 tipo_dato raiz_valor = 0;

	 raiz_valor = arbolin->root->llave;

	 avl_tree_borrar(arbolin, raiz_valor);

	 caca_log_debug("el arbolin despues de borrar raiz\n%s",
	 avl_tree_sprint_identado(arbolin,buffer));

	 avl_tree_borrar(arbolin, estrella_negra);
	 avl_tree_borrar(arbolin, estrella_negra);
	 avl_tree_borrar(arbolin, estrella_negra);

	 caca_log_debug("el arbolin despues de borrar nuevo valor\n%s",
	 avl_tree_sprint_identado(arbolin,buffer));

	 #ifndef ONLINE_JUDGE
	 avl_tree_validar_arbolin_indices(arbolin, arbolin->root);
	 caca_log_debug("validado despues de borrar \n");
	 #endif

	 caca_log_debug("insertando %llu con nodos en arbol %u", estrella_negra,
	 arbolin->nodos_realmente_en_arbol);
	 avl_tree_insert(arbolin, estrella_negra);
	 caca_log_debug("el arbolin despues de insertar de nuez estresha es\n%s",
	 avl_tree_sprint_identado(arbolin,buffer));
	 if (raiz_valor != estrella_negra) {
	 avl_tree_insert(arbolin, raiz_valor);
	 avl_tree_insert(arbolin, raiz_valor);
	 avl_tree_insert(arbolin, raiz_valor);
	 avl_tree_borrar(arbolin, raiz_valor);
	 avl_tree_borrar(arbolin, raiz_valor);
	 }

	 caca_log_debug("el arbolin despues de insertar de nuez es\n%s",
	 avl_tree_sprint_identado(arbolin,buffer));

	 #ifndef ONLINE_JUDGE
	 avl_tree_validar_arbolin_indices(arbolin, arbolin->root);
	 caca_log_debug("validado despues de insertar again\n");
	 #endif

	 num_estrellas_abajo_izq = media_mierda_contar_nodos_izq_aba_descartando(
	 arbolin, estrella_negra);
	 #else
	 num_estrellas_abajo_izq = media_mierda_contar_nodos_izq_aba_iterando(
	 arbolin, estrella_negra);
	 #endif
	 #else
	 num_estrellas_abajo_izq = media_mierda_contar_nodos_izq_aba(
	 arbolin, estrella_negra);
	 #endif
	 conteos_niveles[num_estrellas_abajo_izq]++;
	 }

	 caca_log_debug("los niveles kedaron %s",
	 caca_arreglo_a_cadena(conteos_niveles,num_estrellas,buffer));

	 for (i = 0; i < num_estrellas; i++) {
	 printf("%llu\n", conteos_niveles[i]);
	 }


	 */
	free(numeros);
	free(buffer);
}

int main(int argc, char **argv) {
	media_mierda_main();
	return 0;
}
