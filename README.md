# EI-P3Tests
Repositorio para pruebas unitarias de la práctica 3 de EI

AVISO IMPORTANTE: TODOS LOS FICHEROS FUENTE Y DE PRUEBAS DE ESTA PRÁCTICA DEBERÁN ESTAR CODIFICADOS EN ISO-8859-1 O ISO-8859-15

Para más información, consúltense los siguientes enlaces:

http://stackoverflow.com/questions/64860/best-way-to-convert-text-files-between-character-sets

http://es.wikipedia.org/wiki/ISO_8859-15

http://es.wikipedia.org/wiki/ISO_8859-1

http://es.wikipedia.org/wiki/UTF-8

INSTRUCCIONES PARA LA COMPILACIÓN

Copiar los .h y .cpp en las carpetas include y lib de la raiz del proyecto.

Dar permisos al script practica2.sh

chmod +x practica3.sh

Ejecutar el script:

./practica3.sh

INSTRUCCIONES PARA LA EJECUCIÓN

Ejecutar desde la carpeta raiz del proyecto:

./practica2.sh [-e directorioEntrada] [-r directorioReferencia] [-c directorioCopia]

Siendo:
directorioEntrada -> directorio donde se encuentran los ficheros a indexar.
directorioReferencia -> directorio donde se encuentran los ficheros de salida de referencia.
directorioCopia -> directorio donde se moverá la salida generada por el método IndexarDirectorio y el método Buscar.

Al finalizar la ejecución se creará en la raiz un fichero "reporte_indexacion" y "reporte_busqueda" que resume las diferencias entre la salida generada y la salida esperada
