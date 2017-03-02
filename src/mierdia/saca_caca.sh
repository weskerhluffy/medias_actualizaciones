#!/bin/sh -x
for shit in $(ls ../caca*txt)
do
	echo "ejecutando caso $shit"
	log_arch=${shit%.txt}.log
	echo "el log es $log_arch"

#	python3 actualizacion.py < $shit &> $log_arch
	/programas/python3.5/bin/python3 actualizacion.py < $shit &> $log_arch
	r_code=$?
	if [ $r_code -ne 0 ]
        then
                echo "verga, fallo en $shit"
                exit 1
        fi
done
