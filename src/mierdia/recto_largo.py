'''
Created on 21/02/2017

@author: ernesto
'''
import logging
import sys
nivel_log = logging.ERROR
#nivel_log = logging.DEBUG
logger_cagada = None

def recto_largo_main():
    lineas = list(sys.stdin)
    
    num_numeros = int(lineas[0])
    numeros = [int(x) for x in lineas[1].strip().split(" ")]
    
    numeros.sort()
    
    logger_cagada.debug("los nums %s" % numeros)
    
    area_max = 0
    
    for idx, num in enumerate(numeros):
        nums_restantes = num_numeros - idx
        area_act = num * nums_restantes
        
        if(area_act > area_max):
            area_max = area_act
        
        
    logger_cagada.debug("el area max es %u" % area_max)
    print("%u" % area_max)
        
    
        
if __name__ == '__main__':
    FORMAT = "[%(filename)s:%(lineno)s - %(funcName)20s() ] %(message)s"
    logging.basicConfig(level=nivel_log, format=FORMAT)
    logger_cagada = logging.getLogger("asa")
    logger_cagada.setLevel(nivel_log)

    recto_largo_main()
