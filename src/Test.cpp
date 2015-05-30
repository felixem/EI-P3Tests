/*Copyright (c) 2015 "Félix Escalona Moncholí"
Unit tests for class IndexadorHash

This file is part of https://github.com/felixem/EI-P3Tests.

EI-P3Tests is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include <sys/resource.h>
#include <sys/stat.h>
#include <fstream>
#include <cstring>

#include "buscador.h"
#include "TestIndexador.h"
#include "TestBuscador.h"

//Constantes para ejecución de pruebas
const bool UNIT_TESTS = true;
const bool DIRECTORY_TESTS = true;

//Constantes para el suite de pruebas
const char* LISTA_FICHEROS="lista_fich";
const char* LISTA_DOCUMENTOS="lista_docs";
const char* RESULTADO="resultado";
const char* SALIDA_ESPERADA="salida_esperada";
const char* REPORTE_INDEXACION="reporte_indexacion";
const char* REPORTE_BUSQUEDA="reporte_busqueda";

const char* DIRECTORIO_PRUEBAS ="CorpusTime/Documentos";
const char* DIRECTORIO_SALIDA ="salidas";
const char* DIRECTORIO_COPIA="salida_ejecucion";
const char* DIRECTORIO_DIFERENCIAS="diferencias";
const char* DIRECTORIO_INDEXACION="./indicePrueba";
const char* DIRECTORIO_PREGUNTAS="CorpusTime/Preguntas";

const char* EXTENSION_TK="tk";
const char* EXTENSION_INDEX="index";
const char* EXTENSION_DIFERENCIAS="diff";
const char* EXTENSION_TXT="txt";
const char* EXTENSION_DOCUMENTOS="tim";
const char* EXTENSION_BUSQUEDA="search";

const char* NOMBRE_DFR="DFR";
const char* NOMBRE_BM25="BM25";

const int NUM_DOCUMENTOS_INDEXAR = 50;
const int NUM_DOCUMENTOS_MOSTRAR = 25;
const int NUM_PREGUNTA_INICIO = 1;
const int NUM_PREGUNTA_FIN=83;

//Declaración de funciones
bool borrarListaFicheros(const string &listaFic);

//Suite de tests unitarios
void runSuite(){
	cute::suite suitebuscador;
	cute::suite suiteindexador;

	//Importar la suite de buscador;
	suitebuscador = make_suite_TestBuscador();
	//Importar suite del indexador
	suiteindexador = make_suite_TestIndexador();

	cute::ide_listener lis;
	cute::makeRunner(lis)(suiteindexador, "Suite Indexador");
	cute::makeRunner(lis)(suitebuscador, "Suite Buscador");
}

//Calcular tiempos
double getcputime(void)
{
	struct timeval tim;
	struct rusage ru;
	getrusage(RUSAGE_SELF, &ru);
	tim=ru.ru_utime;
	double t=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
	tim=ru.ru_stime;
	t+=(double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
	return t;
}

//Borrar fichero según su ruta
void borrarFichero(const string &nombreFic)
{
	struct stat dir;
	// Compruebo la existencia del fichero
	int err=stat(nombreFic.c_str(), &dir);
	if(err!=-1)
	{
		//Borrar fichero
		string str = "rm "+nombreFic;
		system(str.c_str());
	}
}

//Comprobar si la entrada se corresponde con un directorio
bool esDirectorio(const string &directorio)
{
	struct stat dir;
	// Compruebo la existencia del directorio
	int err=stat(directorio.c_str(), &dir);
	if(err==-1 || !S_ISDIR(dir.st_mode))
		return false;
	else
		return true;
}

//Volcar la indexación de un directorio en un fichero según un filtro de extensión de archivos
bool indexarDirectorioPorExtension(const string &directorio, const string &extension, const string &nomArchivo)
{
	//Comprobar si es un directorio
	if(!esDirectorio(directorio))
	{
		cerr<<"ERROR: No existe el directorio "<<directorio<<endl;
		return false;
	}
	else
	{
		string cmd="find "+directorio+" -type f -follow -name \"*."+extension+"\" |sort > "+nomArchivo;
		system(cmd.c_str());
		return true;
	}
}

//Limpiar el directorio de los ficheros con la extensión deseada
bool limpiarDirectorio(const string &dirAIndexar, const string &ext)
{
	//Comprobar si es un directorio
	if(!esDirectorio(dirAIndexar))
	{
		cerr<<"ERROR: No existe el directorio "<<dirAIndexar<<endl;
		return false;
	}
	else
	{
		string nombreFic = "borrar_"+ext;
		// Hago una lista de ficheros a borrar con find>fich
		indexarDirectorioPorExtension(dirAIndexar,ext,nombreFic);
		//Borrar la lista de ficheros que se generan en cada ejecución
		borrarListaFicheros(nombreFic);
		//Borrar ficheros auxiliares
		borrarFichero(nombreFic);
	}

	return true;
}

//Borrar lista de ficheros
bool borrarListaFicheros(const string &listaFic)
{
	//Abrir fichero
	ifstream fic;
	bool sinfallos=true;
	fic.open(listaFic.c_str());

	//Fichero inexistente
	if(!fic.is_open())
	{
		cerr << "ERROR: No existe el archivo: "<< listaFic << endl;
		return false;
	}

	//Recorrer el fichero
	string cadena;
	while(getline(fic, cadena))
	{
		borrarFichero(cadena);
	}

	fic.close();
	return sinfallos;
}

//Quitar el primer directorio de la ruta
bool quitarDirectorioComienzo(string &ruta, const string &directorio)
{
	string::size_type pos = ruta.find(directorio+"/");

	//No se pudo quitar el directorio del comienzo
	if(pos == string::npos || pos !=0)
	{
		cerr <<"ERROR: No se pudo borrar el directorio " <<directorio<< " de la ruta "<<ruta<<endl;
		return false;
	}

	//Modificar la ruta
	string::size_type tam = directorio.length()+1;
	ruta.erase(pos,tam);
	return true;
}

//Comparar un fichero con otro y volcar la salida sobre otro fichero
void compararFicheros(const string &fic1, const string &fic2, const string &ficSalida)
{
	string str = "diff "+fic1+" "+fic2+" >> "+ficSalida;
	system(str.c_str());
}

//Comparar lista de ficheros
bool compararListaFicheros(const string &resultado, const string &esperado,
		const string &dirEntrada, const string &dirSalida)
{
	//Abrir fichero
	ifstream res,esp;
	res.open(resultado.c_str());

	//Fichero inexistente
	if(!res.is_open())
	{
		cerr << "ERROR: No existe el archivo: "<< resultado << endl;
		return false;
	}

	//Abrir segundo fichero
	esp.open(esperado.c_str());

	//Fichero inexistente
	if(!esp.is_open())
	{
		cerr << "ERROR: No existe el archivo: "<< esperado << endl;
		res.close();
		return false;
	}

	//Recorrer el fichero
	string cadenaRes;
	string cadenaEsp;
	string rutaRes;
	string rutaEsp;
	bool sinfallos=true;
	int i=0;

	//Comparar todos los ficheros
	while(!esp.eof() && !res.eof())
	{
		cadenaRes="";
		cadenaEsp="";

		//Leer línea
		getline(res, cadenaRes);
		getline(esp, cadenaEsp);

		//Comparar ficheros
		if(cadenaRes.length()!=0 && cadenaEsp.length()!=0)
		{
			//Comparar la ruta de los archivos sin sus directorios raiz
			rutaRes = cadenaRes;
			rutaEsp = cadenaEsp;
			quitarDirectorioComienzo(rutaRes,dirEntrada);
			quitarDirectorioComienzo(rutaEsp,dirSalida);

			//Error en la comparación de rutas
			if(rutaRes != rutaEsp)
			{
				cerr<<"ERROR: El directorio de los ficheros a comparar no coincide."<<
						"Se esperaba "<<rutaEsp<<" pero se obtuvo "<<rutaRes<<endl;
				sinfallos=false;
			}
			//Comparar ficheros
			else
			{
				stringstream dirSalida;
				dirSalida<<DIRECTORIO_DIFERENCIAS<<"/"<<rutaEsp<<"."<<EXTENSION_DIFERENCIAS;
				compararFicheros(cadenaRes,cadenaEsp,dirSalida.str());
			}
		}
		++i;
	}

	//Mala generación en la indexación de ficheros
	if(!res.eof())
	{
		cerr<<"El número de ficheros resultado es mayor del esperado"<<endl;
		sinfallos=false;
	}
	else if(!esp.eof())
	{
		cerr<<"El número de ficheros resultado es menor del esperado"<<endl;
		sinfallos=false;
	}

	//Cerrar ficheros
	esp.close();
	res.close();
	return sinfallos;
}

//Comparar salidas de los ficheros de salida
bool compararSalidas(const string &dirEntrada,const string &dirSalida, const string &extension)
{
	string ficResultados=RESULTADO;
	string ficSalidaEsperada=SALIDA_ESPERADA;
	indexarDirectorioPorExtension(dirEntrada,extension,ficResultados);
	indexarDirectorioPorExtension(dirSalida,extension,ficSalidaEsperada);

	return compararListaFicheros(ficResultados,ficSalidaEsperada,dirEntrada,dirSalida);
}

//Borrar directorio completo
bool borrarDirectorio(const string &directorio)
{
	//Comprobar si es un directorio
	if(esDirectorio(directorio))
	{
		//Borrar directorio
		string cmd ="rm -r "+directorio;
		system(cmd.c_str());
		return true;
	}
	else
		return false;
}

//Calcar la estructura de directorios del origen al destino sin incluir los archivos contenidos
bool calcarEstructuraDirectorios(const string &origen, const string &destino)
{
	//Comprobar si es un directorio
	if(!esDirectorio(origen))
	{
		cerr<<"ERROR: No existe el directorio "<<origen<<endl;
		return false;
	}
	else
	{
		//Copiar estructura de directorio con la salida
		//rsync -r --max-size=1 salida/* diferencias
		string cmd =  "rsync -r --max-size=1 "+origen+"/* "+destino;
		system(cmd.c_str());
		return true;
	}
}

//Borrar archivos de una determinada extension
void borrarArchivosExtension(const string& directorio, const string& ext)
{
	//find prueba -follow -name \"*.tk" | xargs rm -rf
	string cmd="find "+directorio+" -type f -follow -name \"*."+ext+"\" | xargs rm -rf";
	system(cmd.c_str());
}

//Copiar los ficheros de una extension dentro de un directorio a otro directorio respectando la jerarquía de directorios interna
bool copiarArchivosExtension(const string& origen, const string&ext, const string&destino)
{
	//Comprobar si es un directorio
	if(!esDirectorio(origen))
	{
		cerr<<"ERROR: No existe el directorio "<<origen<<endl;
		return false;
	}
	else
	{
		//Copiar todos los ficheros que tengan la extensión de entrada dentro del directorio origen
		//rsync -rv --include '*/' --include '*.tk' --exclude '*' --prune-empty-dirs prueba/* salidaReal
		string cmd =  "rsync -r --include '*/' --include '*."+ext+"' --exclude '*' --prune-empty-dirs "+origen+"/* "+destino;
		system(cmd.c_str());
		//Eliminar los ficheros de la extension en el directorio origen
		borrarArchivosExtension(origen,ext);
		return true;
	}
}

//Limpiar ficheros temporales
void limpiarFicherosTemporales()
{
	borrarFichero(LISTA_FICHEROS);
	borrarFichero(RESULTADO);
	borrarFichero(SALIDA_ESPERADA);
	borrarFichero(LISTA_DOCUMENTOS);
}

//Limpiar directorios y ficheros temporales
void limpiarDirectoriosTemporales(const string& directorioPruebas, const string& directorioSalida,
		const string& directorioComparaciones, const string& directorioCopia)
{
	//Borrar los ficheros temporales generados en cada ejecucion
	limpiarFicherosTemporales();
	borrarFichero(REPORTE_INDEXACION);
	borrarFichero(REPORTE_BUSQUEDA);
	cout<<endl;
	cout<<"Archivos temporales limpiados"<<endl;

	//Limpiar extensiones de ficheros temporales de los directorios
	if(limpiarDirectorio(directorioPruebas,EXTENSION_BUSQUEDA))
		cout<<"Directorio "+directorioPruebas<<" limpiado de ."<<EXTENSION_BUSQUEDA<<endl;

	//Limpiar extensiones de ficheros temporales de los directorios
	if(limpiarDirectorio(directorioPruebas,EXTENSION_INDEX))
		cout<<"Directorio "+directorioPruebas<<" limpiado de ."<<EXTENSION_INDEX<<endl;

	//Limpiar extensiones de ficheros temporales de los directorios
	if(limpiarDirectorio(directorioPruebas,EXTENSION_TK))
		cout<<"Directorio "+directorioPruebas<<" limpiado de ."<<EXTENSION_TK<<endl;

	//Truncar el directorio de diferencias completo
	if(borrarDirectorio(directorioComparaciones))
		cout<<"Directorio "+directorioComparaciones<<" borrado"<<endl;
	//Copiar estructura de directorio con la salida
	if(calcarEstructuraDirectorios(directorioPruebas,directorioComparaciones))
		cout<<"Directorio "+directorioComparaciones+" regenerado"<<endl;
	else
		cerr<<"No se pudo regenerar el directorio "<<directorioComparaciones<<endl;

	//Borrar el directorio de copia
	if(borrarDirectorio(directorioCopia))
		cout<<"Directorio "+directorioCopia<<" borrado"<<endl;

}

//Método para testear la indexación de directorios
bool testIndexarDirectorio(IndexadorHash &ind, const string& directorioPruebas,
		const string& directorioSalida, long double &tiempoFinal)
{
	bool sinerrores = true;

	//Indexar el directorio
	if(!ind.IndexarDirectorio(directorioPruebas))
	{
		cerr<<"Hubo problemas graves en la indexación del directorio"<<endl;
		sinerrores = false;
		tiempoFinal = getcputime();
	}

	//Serializar directorio
	else if(!ind.GuardarIndexacion())
	{
		cerr<<"Hubo problemas en la serialización del directorio"<<endl;
		sinerrores=false;
	}

	tiempoFinal = getcputime();
	return sinerrores;
}

//Método para testear la búsqueda de preguntas
bool testBuscadorDirectorio(const string &dirEntrada, long double &tiempoFinal)
{
	bool sinerrores = true;

	//Realizar búsqueda en DFR
	Buscador buscador(DIRECTORIO_INDEXACION,0);

	//Comprobar si la búsqueda se realiza correctamente
	if(buscador.Buscar(DIRECTORIO_PREGUNTAS,NUM_DOCUMENTOS_INDEXAR,
			NUM_PREGUNTA_INICIO,NUM_PREGUNTA_FIN))
	{
		//Construir nombre de fichero busqueda
		stringstream ss;
		ss << dirEntrada << "/" << NOMBRE_DFR << "." <<EXTENSION_BUSQUEDA;
		//Comprobar si el resultado se imprime correctamente
		if(!buscador.ImprimirResultadoBusqueda(NUM_DOCUMENTOS_MOSTRAR,ss.str()))
		{
			cerr<<"Hubo problemas para imprimir los resultados de la búsqueda DFR en "<<ss.str()<<endl;
			sinerrores=false;
		}
	}
	else
	{
		cerr <<"Hubo problemas en la búsqueda DFR"<<endl;
		sinerrores = false;
	}

	//Realizar búsqueda en BM25
	buscador.CambiarFormulaSimilitud(1);
	//Comprobar si la búsqueda se realiza correctamente
	if(buscador.Buscar(DIRECTORIO_PREGUNTAS,NUM_DOCUMENTOS_INDEXAR,
			NUM_PREGUNTA_INICIO,NUM_PREGUNTA_FIN))
	{
		//Construir nombre de fichero busqueda
		stringstream ss;
		ss << dirEntrada << "/" << NOMBRE_BM25 << "." <<EXTENSION_BUSQUEDA;

		//Comprobar si el resultado se imprime correctamente
		if(!buscador.ImprimirResultadoBusqueda(NUM_DOCUMENTOS_MOSTRAR,ss.str()))
		{
			cerr<<"Hubo problemas para imprimir los resultados de la búsqueda BM26 en "<<ss.str()<<endl;
			sinerrores=false;
		}
	}
	else
	{
		cerr <<"Hubo problemas en la búsqueda BM25"<<endl;
		sinerrores = false;
	}

	tiempoFinal = getcputime();
	return sinerrores;
}

//Saber si un fichero está vacío
bool esFicheroVacio(const string &fichero)
{
	struct stat dat;

	//Comprobar si se puede abrir el fichero
	if(stat(fichero.c_str(),&dat) == -1) {
		cerr<<"ERROR: no se pudo abrir el fichero "<<fichero<<endl;
		return true;
	}

	//Comprobar la cantidad de bytes
	if(dat.st_size == 0)
		return true;
	else
		return false;
}

//Escribir en el informe los ficheros que no estén vacíos
bool reportarNoVacios(const string &listado, const string &nomSalida)
{
	ifstream ficEntrada;
	ofstream ficSalida;
	bool sinfallos=true;

	//Abrir listado de ficheros
	ficEntrada.open(listado.c_str());
	//Comprobar la apertura del listado
	if(ficEntrada.is_open())
	{
		ficSalida.open(nomSalida.c_str());
		//Comprobar creación del archivo de reporte
		if(ficSalida.is_open())
		{
			//Añadir un título dentro del reporte
			ficSalida<<"Listado de ficheros de diferencia que reportan errores:"<<"\n";

			//Recorrer el fichero mientras no se llegue al final
			string cadena;
			while(getline(ficEntrada,cadena))
			{
				//Añadir en el reporte los ficheros no vacíos
				if(!esFicheroVacio(cadena))
					ficSalida<<cadena<<"\n";
				//Borrar fichero en caso de estar vacío
				else
					borrarFichero(cadena);
			}
		}
		//Error en el fichero de salida
		else
		{
			cerr<<"ERROR: No se pudo generar el fichero "<<listado<<endl;
			sinfallos=false;
		}
	}
	else
	{
		cerr<<"ERROR: No se pudo abrir el fichero "<<listado<<endl;
		sinfallos=false;
	}

	ficEntrada.close();
	ficSalida.close();

	return sinfallos;
}

//Genera un informe sobre los ficheros de diferencias situados en el directorio de entrada
bool generarInforme(const string &directorio,const string& reporte)
{
	string listado ="listado_diferencias";

	//Generar un listado con todos los archivos diff
	indexarDirectorioPorExtension(directorio,EXTENSION_DIFERENCIAS,listado);
	//Generar el reporte de diferencias
	bool sinfallos=reportarNoVacios(listado,reporte);
	//Eliminar el fichero temporal de listado de diferencias
	borrarFichero(listado);

	return sinfallos;
}

//Volcar información de la indexación de cada documento en archivos .index
bool volcarIndexacion(const IndexadorHash &ind, const string &dirEntrada, long double &tiempo)
{
	string ficListaDocs=LISTA_DOCUMENTOS;

	bool correcto = indexarDirectorioPorExtension(dirEntrada,EXTENSION_DOCUMENTOS,ficListaDocs);

	//Realizar el volcado
	ifstream file;
	file.open(LISTA_DOCUMENTOS);

	if(!file)
	{
		cerr<<"ERROR: No se pudo abrir el fichero "<<ficListaDocs<<endl;
		tiempo=getcputime();
		return false;
	}

	string cadena;

	//Recorrer todos los documentos
	while(getline(file,cadena))
	{
		ofstream salida;
		salida.open(cadena+"."+EXTENSION_INDEX);

		if(!salida)
		{
			cerr << "ERROR: No se pudo volcar la indexación del fichero "<<cadena;
			correcto = false;
			continue;
		}

		//Salvar el buffer de cout
		std::streambuf *sbuf = std::cout.rdbuf();
		// Redirigir cout hacia el buffer del fichero
		std::cout.rdbuf(salida.rdbuf());

		//Listar los términos del documento
		ind.ListarDocs(cadena);

		//Redirigir cout a su buffer original
		std::cout.rdbuf(sbuf);
		//Cerrar el fichero de salida
		salida.close();;
	}

	file.close();

	tiempo=getcputime();

	return correcto;
}

//Probar la indexación de directorios
int runSuiteTemporal(const string& directorioPruebas, const string& directorioSalida, const string& directorioCopia)
{
	string directorioComparaciones =DIRECTORIO_DIFERENCIAS;

	//Limpiar temporales
	limpiarDirectoriosTemporales(directorioPruebas,directorioSalida,directorioComparaciones,directorioCopia);

	//Indexación de directorio
	cout <<"------------------------------------------------"<<endl;
	cout <<"Iniciando indexación del directorio "+directorioPruebas<<endl;

	//Medir el tiempo de indexación
	long double tiempo;
	long double aa = getcputime();

	//Declarar indexador
	IndexadorHash ind("./StopWordsEspanyol.txt", " .,:;/-@*(){}?!|[]'=_\"><&#-\r\t", false, false, "./indicePrueba", 0, false, true);

	//Indexar directorio
	if(testIndexarDirectorio(ind,directorioPruebas,directorioSalida,tiempo))
	{
		//Mostrar resultado temporal
		cout << "Ha tardado " << tiempo - aa << " segundos en indexar el directorio" << endl;

		//Volcar contenido de la indexación de cada documento
		cout <<"Volcando indexación de cada documento"<<endl;
		aa = getcputime();

		volcarIndexacion(ind,directorioPruebas, tiempo);
		//Mostrar resultado temporal en el volcado de la indexación
		cout << "Ha tardado " << tiempo - aa << " segundos en volcar la indexación del directorio" << endl;

		//Comprobar errores de comparación
		if(compararSalidas(directorioPruebas,directorioSalida,EXTENSION_INDEX))
		{
			//Generar un fichero de resumen sobre las diferencias entre la salida esperada y la generada por la indexación
			string reporte =REPORTE_INDEXACION;
			if(generarInforme(directorioComparaciones,reporte))
				cout<<"Informe de comparaciones generado en "<<reporte<<endl;
		}
		else
			cerr<<"Hubo problemas en la comparación de los ficheros generados"<<endl;

		//Hacer una copia de los ficheros .index generados a otra carpeta para poder usarla como referencia en próximas ejecuciones
		cout <<"Haciendo copia de los ficheros .index generados hacia "<<directorioCopia<<endl;

		if(copiarArchivosExtension(directorioPruebas,EXTENSION_INDEX,directorioCopia))
			cout<<"Salida de la indexación movida desde "<<directorioPruebas<<" hacia "<<directorioCopia<<endl;
		else
			cerr<<"No pudieron ser movidos los ficheros .index hacia "<<directorioCopia<<endl;


		//PRUEBAS DE BÚSQUEDA
		cout <<"------------------------------------------------"<<endl;
		cout <<"Iniciando búsqueda de preguntas de "+string(DIRECTORIO_PREGUNTAS)<<endl;
		//Realizar pruebas de búsqueda
		aa = getcputime();
		if(testBuscadorDirectorio(directorioPruebas, tiempo))
		{
			//Mostrar resultado temporal
			cout << "Ha tardado " << tiempo - aa << " segundos en realizar la búsqueda y volcar su contenido" << endl;

			//Comprobar errores de comparación
			if(compararSalidas(directorioPruebas,directorioSalida,EXTENSION_BUSQUEDA))
			{
				//Generar un fichero de resumen sobre las diferencias entre la salida esperada y la generada por la indexación
				string reporte =REPORTE_BUSQUEDA;
				if(generarInforme(directorioComparaciones,reporte))
					cout<<"Informe de comparaciones generado en "<<reporte<<endl;
			}
			else
				cerr<<"Hubo problemas en la comparación de los ficheros generados"<<endl;

			//Hacer una copia de los ficheros search generados a otra carpeta para poder usarla como referencia en próximas ejecuciones
			cout <<"Haciendo copia de los ficheros .search generados hacia "<<directorioCopia<<endl;

			if(copiarArchivosExtension(directorioPruebas,EXTENSION_BUSQUEDA,directorioCopia))
				cout<<"Salida de la búsqueda movida desde "<<directorioPruebas<<" hacia "<<directorioCopia<<endl;
			else
				cerr<<"No pudieron ser movidos los ficheros .search hacia "<<directorioCopia<<endl;
		}
		else
		{
			cerr<<"Se produjeron errores durante la búsqueda de las preguntas"<<endl;
			return -1;
		}

		//Limpiar ficheros temporales
		limpiarFicherosTemporales();
	}
	//Errores de indexación
	else
	{
		cerr<<"Se produjeron errores durante la indexación del directorio"<<endl;
		return -1;
	}



	return 0;
}

//Quitar caracter final
void quitarCaracterFinal (string &cadena, const char &car)
{
	//Cadena vacía
	if(!cadena.empty())
	{
		//Borrar caracter si es el introducido por parámetro
		string::size_type posFin = cadena.length()-1;
		if(cadena[posFin] == car)
			cadena.resize(posFin);
	}
}

//Comprobar los argumentos recibidos como parámetro
bool comprobarArgumentos(const int &argc, char** argv,
		string &directorioPruebas, string &directorioSalida,
		string &directorioCopia)
{
	//Comprobación de parámetros
	if(argc == 1)
		return true;
	//El número de parámetros debe ser impar (el primer parámetro es el nombre del programa)
	if(argc > 7 || argc%2==0)
	{
		cerr<<"ERROR: Número de parámetros inválido"<<endl;
		return false;
	}

	int i=1;
	//Bucle de reconocimiento de parámetros
	while(i<argc)
	{
		//Opción
		char * cad = argv[i];

		//Directorio de entrada de las pruebas
		if(strcmp(cad,"-e")==0)
		{
			++i;
			directorioPruebas =argv[i];
			quitarCaracterFinal(directorioPruebas,'/');
		}
		//Directorio de referencia para las pruebas
		else if(strcmp(cad,"-r")==0)
		{
			++i;
			directorioSalida =argv[i];
			quitarCaracterFinal(directorioSalida,'/');
		}
		//Directorio donde copiar la salida de la indexación
		else if(strcmp(cad,"-c")==0)
		{
			++i;
			directorioCopia=argv[i];
			quitarCaracterFinal(directorioCopia,'/');
		}
		//Parámetro erróneo
		else
		{
			cerr<<"ERROR: Parámetro "<<cad<<" desconocido"<<endl;
			return false;
		}

		++i;
	}
	return true;
}


//Método de prueba estándar y con tiempo (Pasar el valgrind para el consumo de memoria)
int main(int argc, char** argv)
{
	//Pruebas de indexación individual
	if(UNIT_TESTS)
		runSuite();

	//Indexación de directorios
	string directorioPruebas =DIRECTORIO_PRUEBAS;
	string directorioSalida =DIRECTORIO_SALIDA;
	string directorioCopia=DIRECTORIO_COPIA;

	//Comprobación de parámetros
	if(!comprobarArgumentos(argc,argv,directorioPruebas,directorioSalida,directorioCopia))
	{
		cout<<"USO: "<<argv[0]<<" [-e directorioEntrada] [-r directorioReferencia] [-c directorioCopia] "<<endl;
		return -1;
	}

	//Lanzar la suite de indexación de directorios
	if(DIRECTORY_TESTS)
		runSuiteTemporal(directorioPruebas, directorioSalida, directorioCopia);

	return 0;
}



