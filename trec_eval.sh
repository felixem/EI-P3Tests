#! /bin/bash

echo "---------------------------------------------------"
echo "Limpiando análisis anteriores realizados .res con trec_eval en el directorio resultados_trec_eval"
#Realizar limpieza del directorio
find "resultados_trec_eval" -type f -follow -name \"*.res\" | xargs rm -rf
#Ejecutamos trec_eval para DFR
echo "Analizando resultados para DFR"
trec_eval_8_1_linux/trec_eval -q -o resultados_trec_eval/frelevancia_trec_eval_TIME.txt salida_ejecucion/DFR.search > resultados_trec_eval/trec_eval_DFR.res
echo "Resultado del análisis de la búsqueda almacenado en resultados_trec_eval/trec_eval_DFR.res"
#Ejecutamos trec_eval para BM25
echo "Analizando resultados para BM25"
trec_eval_8_1_linux/trec_eval -q -o resultados_trec_eval/frelevancia_trec_eval_TIME.txt salida_ejecucion/BM25.search > resultados_trec_eval/trec_eval_BM25.res
echo "Resultado del análisis de la búsqueda almacenado en resultados_trec_eval/trec_eval_BM25.res"


