'''
Created on 17/02/2017

@author: ernesto
'''
import logging
import sys

nivel_log = logging.ERROR
nivel_log = logging.DEBUG
logger_cagada = None

class Node():
    def __init__(self, key):
        self.key = key
        self.parent = None
        self.leftChild = None
        self.rightChild = None
        self.height = 0 
        self.num_hijos = 0
    
    def __str__(self):
        return str(self.key) + "(" + str(self.height) + ")"
    
    def is_leaf(self):
        return (self.height == 0)
   
    def max_children_height(self):
        if self.leftChild and self.rightChild:
            return max(self.leftChild.height, self.rightChild.height)
        elif self.leftChild and not self.rightChild:
            return self.leftChild.height
        elif not self.leftChild and  self.rightChild:
            return self.rightChild.height
        else:
            return -1
        
    def balance (self):
        return (self.leftChild.height if self.leftChild else -1) - (self.rightChild.height if self.rightChild else -1)

class AVLTree():
    def __init__(self, *args):
        self.rootNode = None
        self.elements_count = 0
        self.rebalance_count = 0
        if len(args) == 1:
            for i in args[0]:
                self.insert (i)
        
    def height(self):
        if self.rootNode:
            return self.rootNode.height
        else:
            return 0
        
    def rebalance (self, node_to_rebalance):
        self.rebalance_count += 1
        A = node_to_rebalance 
        F = A.parent  # allowed to be NULL
        if node_to_rebalance.balance() == -2:
            if node_to_rebalance.rightChild.balance() <= 0:
                """Rebalance, case RRC """
                B = A.rightChild
                C = B.rightChild
                assert (not A is None and not B is None and not C is None)
                A.rightChild = B.leftChild
                if A.rightChild:
                    A.rightChild.parent = A
                B.leftChild = A
                A.parent = B                                                               
                if F is None:                                                              
                    self.rootNode = B 
                    self.rootNode.parent = None                                                   
                else:                                                                        
                    if F.rightChild == A:                                                          
                        F.rightChild = B                                                                  
                    else:                                                                      
                        F.leftChild = B                                                                   
                    B.parent = F 
                self.recompute_heights (A) 
                self.recompute_heights (B.parent)                                                                                         
            else:
                """Rebalance, case RLC """
                B = A.rightChild
                C = B.leftChild
                assert (not A is None and not B is None and not C is None)
                B.leftChild = C.rightChild
                if B.leftChild:
                    B.leftChild.parent = B
                A.rightChild = C.leftChild
                if A.rightChild:
                    A.rightChild.parent = A
                C.rightChild = B
                B.parent = C                                                               
                C.leftChild = A
                A.parent = C                                                             
                if F is None:                                                             
                    self.rootNode = C
                    self.rootNode.parent = None                                                    
                else:                                                                        
                    if F.rightChild == A:                                                         
                        F.rightChild = C                                                                                     
                    else:                                                                      
                        F.leftChild = C
                    C.parent = F
                self.recompute_heights (A)
                self.recompute_heights (B)
        else:
            assert(node_to_rebalance.balance() == +2)
            if node_to_rebalance.leftChild.balance() >= 0:
                B = A.leftChild
                C = B.leftChild
                """Rebalance, case LLC """
                assert (not A is None and not B is None and not C is None)
                A.leftChild = B.rightChild
                if (A.leftChild): 
                    A.leftChild.parent = A
                B.rightChild = A
                A.parent = B
                if F is None:
                    self.rootNode = B
                    self.rootNode.parent = None                    
                else:
                    if F.rightChild == A:
                        F.rightChild = B
                    else:
                        F.leftChild = B
                    B.parent = F
                self.recompute_heights (A)
                self.recompute_heights (B.parent) 
            else:
                B = A.leftChild
                C = B.rightChild 
                """Rebalance, case LRC """
                assert (not A is None and not B is None and not C is None)
                A.leftChild = C.rightChild
                if A.leftChild:
                    A.leftChild.parent = A
                B.rightChild = C.leftChild
                if B.rightChild:
                    B.rightChild.parent = B
                C.leftChild = B
                B.parent = C
                C.rightChild = A
                A.parent = C
                if F is None:
                    self.rootNode = C
                    self.rootNode.parent = None
                else:
                    if (F.rightChild == A):
                        F.rightChild = C
                    else:
                        F.leftChild = C
                    C.parent = F
                self.recompute_heights (A)
                self.recompute_heights (B)
                
    def sanity_check (self, *args):
        if len(args) == 0:
            node = self.rootNode
        else:
            node = args[0]
        if (node  is None) or (node.is_leaf() and node.parent is None):
            # trival - no sanity check needed, as either the tree is empty or there is only one node in the tree     
            pass    
        else:
            if node.height != node.max_children_height() + 1:
                raise Exception ("Invalid height for node " + str(node) + ": " + str(node.height) + " instead of " + str(node.max_children_height() + 1) + "!")
            
            num_hijos_nodo = (node.leftChild.num_hijos + 1 if node.leftChild else 0) + (node.rightChild.num_hijos + 1 if node.rightChild else 0)
            if node.num_hijos != num_hijos_nodo:
                raise Exception ("Invalid num_ijos para node " + str(node) + ": " + str(node.num_hijos) + " instead of " + str(num_hijos_nodo) + "!")
                
            balFactor = node.balance()
            # Test the balance factor
            if not (balFactor >= -1 and balFactor <= 1):
                raise Exception ("Balance factor for node " + str(node) + " is " + str(balFactor) + "!")
            # Make sure we have no circular references
            if not (node.leftChild != node):
                raise Exception ("Circular reference for node " + str(node) + ": node.leftChild is node!")
            if not (node.rightChild != node):
                raise Exception ("Circular reference for node " + str(node) + ": node.rightChild is node!")
            
            if (node.leftChild): 
                if not (node.leftChild.parent == node):
                    raise Exception ("Left child of node " + str(node) + " doesn't know who his father is!")
                if not (node.leftChild.key <= node.key):
                    raise Exception ("Key of left child of node " + str(node) + " is greater than key of his parent!")
                self.sanity_check(node.leftChild)
            
            if (node.rightChild): 
                if not (node.rightChild.parent == node):
                    raise Exception ("Right child of node " + str(node) + " doesn't know who his father is!")
                if not (node.rightChild.key >= node.key):
                    raise Exception ("Key of right child of node " + str(node) + " is less than key of his parent!")
                self.sanity_check(node.rightChild)
            
    def recompute_heights (self, start_from_node):
        changed = True
        node = start_from_node
        while node and changed:
            old_height = node.height
            node.height = (node.max_children_height() + 1 if (node.rightChild or node.leftChild) else 0)
            changed = node.height != old_height
            node = node.parent
        
        self.recomputa_num_hijos(start_from_node)
    
    def recomputa_num_hijos(self, starteando):
        node = starteando
        while node:
            node.num_hijos = (node.leftChild.num_hijos + 1 if node.leftChild else 0) + (node.rightChild.num_hijos + 1 if node.rightChild else 0)
            node = node.parent
       
    def add_as_child (self, parent_node, child_node):
        node_to_rebalance = None
        if child_node.key < parent_node.key:
            if not parent_node.leftChild:
                parent_node.leftChild = child_node
                child_node.parent = parent_node
                if parent_node.height == 0:
                    node = parent_node
                    while node:
                        node.height = node.max_children_height() + 1
                        if not node.balance () in [-1, 0, 1]:
                            node_to_rebalance = node
                            break  # we need the one that is furthest from the root
                        node = node.parent     
            else:
                self.add_as_child(parent_node.leftChild, child_node)
        else:
            if not parent_node.rightChild:
                parent_node.rightChild = child_node
                child_node.parent = parent_node
                if parent_node.height == 0:
                    node = parent_node
                    while node:
                        node.height = node.max_children_height() + 1
                        if not node.balance () in [-1, 0, 1]:
                            node_to_rebalance = node
                            break  # we need the one that is furthest from the root
                        node = node.parent       
            else:
                self.add_as_child(parent_node.rightChild, child_node)
        
        if node_to_rebalance:
            self.rebalance (node_to_rebalance)
        else:
            self.recomputa_num_hijos(child_node)

        
    
    def insert (self, key):
        new_node = Node (key)
        if not self.rootNode:
            self.rootNode = new_node
        else:
#       if not self.find(key):
            self.add_as_child (self.rootNode, new_node)
        self.elements_count += 1
                
#        print("el num d ijos d root %u" % self.rootNode.num_hijos)
      
    def find_biggest(self, start_node):
        node = start_node
        while node.rightChild:
            node = node.rightChild
        return node 
    
    def find_smallest(self, start_node):
        node = start_node
        while node.leftChild:
            node = node.leftChild
        return node
     
    def inorder_non_recursive (self):
        node = self.rootNode
        retlst = []
        while node.leftChild:
            node = node.leftChild
        while (node):
            retlst += [node.key]
            if (node.rightChild):
                node = node.rightChild
                while node.leftChild:
                    node = node.leftChild
            else:
                while ((node.parent)  and (node == node.parent.rightChild)):
                    node = node.parent
                node = node.parent
        return retlst
 
    def preorder(self, node, retlst=None):
        if retlst is None:
            retlst = []
        retlst += [node.key]
        if node.leftChild:
            retlst = self.preorder(node.leftChild, retlst) 
        if node.rightChild:
            retlst = self.preorder(node.rightChild, retlst)
        return retlst         
           
    def inorder(self, node, retlst=None):
        if retlst is None:
            retlst = [] 
        if node.leftChild:
            retlst = self.inorder(node.leftChild, retlst)
        retlst += [node.key] 
        if node.rightChild:
            retlst = self.inorder(node.rightChild, retlst)
        return retlst
        
    def postorder(self, node, retlst=None):
        if retlst is None:
            retlst = []
        if node.leftChild:
            retlst = self.postorder(node.leftChild, retlst) 
        if node.rightChild:
            retlst = self.postorder(node.rightChild, retlst)
        retlst += [node.key]
        return retlst  
    
    def as_list (self, pre_in_post):
        if not self.rootNode:
            return []
        if pre_in_post == 0:
            return self.preorder (self.rootNode)
        elif pre_in_post == 1:
            return self.inorder (self.rootNode)
        elif pre_in_post == 2:
            return self.postorder (self.rootNode)
        elif pre_in_post == 3:
            return self.inorder_non_recursive()      
          
    def find(self, key):
        return self.find_in_subtree (self.rootNode, key)
    
    def find_in_subtree (self, node, key):
        if node is None:
            return None  # key not found
        if key < node.key:
            return self.find_in_subtree(node.leftChild, key)
        elif key > node.key:
            return self.find_in_subtree(node.rightChild, key)
        else:  # key is equal to node key
            return node
    
    def remove (self, key):
        # first find
        llave_borrada = None
        node = self.find(key)
        
        if not node is None:
            llave_borrada = key
            self.elements_count -= 1
            
            #     There are three cases:
            # 
            #     1) The node is a leaf.  Remove it and return.
            # 
            #     2) The node is a branch (has only 1 child). Make the pointer to this node 
            #        point to the child of this node.
            # 
            #     3) The node has two children. Swap items with the successor
            #        of the node (the smallest item in its right subtree) and
            #        delete the successor from the right subtree of the node.
            if node.is_leaf():
                logger_cagada.debug("removiendo una hoja %u" % key)
                self.remove_leaf(node)
            elif (bool(node.leftChild)) ^ (bool(node.rightChild)):  
                logger_cagada.debug("removiendo una rama %u" % key)
                self.remove_branch (node)
            else:
                logger_cagada.debug("removiendo un desmadre %u" % key)
                assert (node.leftChild) and (node.rightChild)
                self.swap_with_successor_and_remove (node)
                
        return llave_borrada
            
    def remove_leaf (self, node):
        parent = node.parent
        logger_cagada.debug("removiendo hoja %u de padre %s" % (node.key, parent))
        if (parent):
            if parent.leftChild == node:
                parent.leftChild = None
            else:
                assert (parent.rightChild == node)
                parent.rightChild = None
            self.recompute_heights(parent)
        else:
            self.rootNode = None
        del node
        # rebalance
        node = parent
        while (node):
            if not node.balance() in [-1, 0, 1]:
                self.rebalance(node)
            node = node.parent
        
        
    def remove_branch (self, node):
        parent = node.parent
        logger_cagada.debug("padre es %s" % parent)
        if (parent):
            if parent.leftChild == node:
                parent.leftChild = node.rightChild or node.leftChild
            else:
                assert (parent.rightChild == node)
                parent.rightChild = node.rightChild or node.leftChild
            if node.leftChild:
                node.leftChild.parent = parent
            else:
                assert (node.rightChild)
                node.rightChild.parent = parent 
            self.recompute_heights(parent)
        # rebalance
        if(not parent):
            self.rootNode = node.rightChild or node.leftChild
            self.rootNode.parent = None
        else:
            node = parent
            while (node):
                if not node.balance() in [-1, 0, 1]:
                    self.rebalance(node)
                node = node.parent
            
        logger_cagada.debug("borrando cabron el nodo %u" % node.key)
        node.key = None
        node.leftChild = None
        node.rightChild = None
        node.parent = None
        del node
        
        
    def swap_with_successor_and_remove (self, node):
        successor = self.find_smallest(node.rightChild)
        self.swap_nodes (node, successor)
        assert (node.leftChild is None)
        if node.height == 0:
            self.remove_leaf (node)
        else:
            self.remove_branch (node)
            
    def swap_nodes (self, node1, node2):
        assert (node1.height > node2.height)
        parent1 = node1.parent
        leftChild1 = node1.leftChild
        rightChild1 = node1.rightChild
        parent2 = node2.parent
        assert (not parent2 is None)
        assert (parent2.leftChild == node2 or parent2 == node1)
        leftChild2 = node2.leftChild
        assert (leftChild2 is None)
        rightChild2 = node2.rightChild
        
        # swap heights
        tmp = node1.height 
        node1.height = node2.height
        node2.height = tmp
       
        if parent1:
            if parent1.leftChild == node1:
                parent1.leftChild = node2
            else:
                assert (parent1.rightChild == node1)
                parent1.rightChild = node2
            node2.parent = parent1
        else:
            self.rootNode = node2
            node2.parent = None
            
        node2.leftChild = leftChild1
        leftChild1.parent = node2
        node1.leftChild = leftChild2  # None
        node1.rightChild = rightChild2
        if rightChild2:
            rightChild2.parent = node1 
        if not (parent2 == node1):
            node2.rightChild = rightChild1
            rightChild1.parent = node2
            
            parent2.leftChild = node1
            node1.parent = parent2
        else:
            node2.rightChild = node1
            node1.parent = node2           
           
    # use for debug only and only with small trees            
    def out(self, start_node=None):
        if start_node == None:
            start_node = self.rootNode
        space_symbol = "*"
        spaces_count = 80
        out_string = ""
        initial_spaces_string = space_symbol * spaces_count + "\n" 
        if not start_node:
            return "AVLTree is empty"
        else:
            level = [start_node]
            while (len([i for i in level if (not i is None)]) > 0):
                level_string = initial_spaces_string
                for i in range(len(level)):
                    j = (i + 1) * spaces_count / (len(level) + 1)
                    level_string = level_string[:j] + (str(level[i]) if level[i] else space_symbol) + level_string[j + 1:]
                level_next = []
                for i in level:
                    level_next += ([i.leftChild, i.rightChild] if i else [None, None])
                level = level_next
                out_string += level_string                    
        return out_string
    
    def recorrer_in_order_de_reversa_mami(self, num_posiciones):
        num_en_pos = None
        num_recorridos = -1
        ya_imprimidos = set()
        num_ant = 0
        
        nodo_act = self.rootNode
        num_ant = nodo_act.key
        
        while(nodo_act):
            if(nodo_act.key not in ya_imprimidos):
#                print("you could be mine %u" % nodo_act.key)
                ya_imprimidos.add(nodo_act.key)
                num_recorridos += 1
                if(num_recorridos == num_posiciones):
                    num_en_pos = nodo_act.key
                    break
            if(nodo_act.leftChild and nodo_act.leftChild.key not in ya_imprimidos): 
                nodo_act = nodo_act.leftChild
                while(nodo_act.rightChild):
                    nodo_act = nodo_act.rightChild
            else:
                nodo_act = nodo_act.parent
                
            assert num_ant >= nodo_act.key, "ke bergha el ant %u el act %u" % (num_ant, nodo_act.key)
        
        return num_en_pos
                    
    def recorrer_in_order(self, num_posiciones):
        num_en_pos = None
        num_recorridos = -1
        ya_imprimidos = set()
        num_ant = 0
        
        nodo_act = self.rootNode
        num_ant = nodo_act.key
        
        while(nodo_act):
            if(nodo_act.key not in ya_imprimidos):
#                print("you could be mine %u" % nodo_act.key)
                ya_imprimidos.add(nodo_act.key)
                num_recorridos += 1
                if(num_recorridos == num_posiciones):
                    num_en_pos = nodo_act.key
                    break
            if(nodo_act.rightChild and nodo_act.rightChild.key not in ya_imprimidos): 
                nodo_act = nodo_act.rightChild
                while(nodo_act.leftChild):
                    nodo_act = nodo_act.leftChild
            else:
                nodo_act = nodo_act.parent
                
            assert num_ant <= nodo_act.key, "ke bergha el ant %u el act %u" % (num_ant, nodo_act.key)
        
        return num_en_pos

def mierdia_actualizacion_core(arbolin, numero, eliminar):
    mierdia = None
    mierdia_par = None
    num_cacas = arbolin.elements_count
    
    assert num_cacas == (arbolin.rootNode.num_hijos + 1 if arbolin.rootNode else 0) , "el num de cacas %u, el num de ijos %u, los elems %s" % (num_cacas, ((arbolin.rootNode.num_hijos + 1 if arbolin.rootNode else 0)), arbolin.as_list(1))
    
    if(eliminar):
        logger_cagada.debug("eleiminando %u de %s" % (numero, arbolin.as_list(1)))
        if(num_cacas):
            eliminada = arbolin.remove(numero)
            if(eliminada):
                mierdia = eliminada
                num_cacas -= 1
                logger_cagada.debug("eleiminado %u definitivamente, aora kedan %u(%u) elems %s" % (numero, num_cacas, arbolin.elements_count, arbolin.as_list(1)))
    else:
        arbolin.insert(numero)
        mierdia = numero
        num_cacas += 1
        logger_cagada.debug("insertado %u, aora el num de cacas %u (%u)" % (numero, num_cacas, arbolin.elements_count))
    
    logger_cagada.debug("mierdia es %s" % mierdia)
    if(mierdia is not None and num_cacas):
        logger_cagada.debug("de echo se izo algo")
        if(num_cacas == 1):
            return arbolin.rootNode.key
        if(num_cacas == 2):
            return (arbolin.rootNode.key + (arbolin.rootNode.leftChild.key if arbolin.rootNode.leftChild else arbolin.rootNode.rightChild.key)) / 2
        
        pos_mierdia = num_cacas >> 1
        
        logger_cagada.debug("la pos de mierdia %u" % pos_mierdia)
        
        pos_raiz = arbolin.rootNode.leftChild.num_hijos + 1 if arbolin.rootNode.leftChild else 0
        logger_cagada.debug("la pos de raiz %u" % pos_raiz)
    
        desfase = pos_mierdia - pos_raiz
        
        logger_cagada.debug("el desfase de mierdia %u" % desfase)
        
        if(desfase < 0):
            recorrido = arbolin.recorrer_in_order_de_reversa_mami
        else:
            recorrido = arbolin.recorrer_in_order
            
        desfase = abs(desfase)
        
        mierdia = recorrido(desfase)
        logger_cagada.debug("la mierdia es %u" % mierdia)
        if(not (num_cacas % 2)):
            mierdia_par = recorrido(desfase - 1)
            logger_cagada.debug("la mierdia par es %u" % mierdia_par)
        
        if(mierdia_par):
            mierdia += mierdia_par
            mierdia /= 2
            
        logger_cagada.debug("la mierdia definitiva es %u" % mierdia)
    
    if(not num_cacas):
        mierdia = None
    
    return mierdia
    
def mierdia_actualizacion_main():
    lineas = list(sys.stdin)
    arbolin = AVLTree()
    
    num_numeros = int(lineas[0])
    
    for linea in lineas[1:]:
        operacion, numero_str = linea.strip().split(" ")
        numero = int(numero_str)
        
        logger_cagada.debug("la operacoin %s el numero %u" % (operacion, numero))
        
        medio_ombre = mierdia_actualizacion_core(arbolin, numero, operacion == "r")
        
        logger_cagada.debug("la mierdia regresada %s" % medio_ombre)
        print("%s" % (medio_ombre if medio_ombre else "Wrong!"))
        
if __name__ == '__main__':
    FORMAT = "[%(filename)s:%(lineno)s - %(funcName)20s() ] %(message)s"
    logging.basicConfig(level=nivel_log, format=FORMAT)
    logger_cagada = logging.getLogger("asa")
    logger_cagada.setLevel(nivel_log)

    mierdia_actualizacion_main()
