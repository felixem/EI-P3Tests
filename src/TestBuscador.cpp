#include "TestBuscador.h"
#include "buscador.h"

#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include <sys/resource.h>
#include <sys/stat.h>
#include <fstream>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

//Tests del profesor
namespace Profesor
{

void buscador01() {
	IndexadorHash b("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, false);

	ASSERT_EQUAL(true,b.Indexar("./listaFicheros_corto.txt"));
	ASSERT_EQUAL(true,b.GuardarIndexacion());

	Buscador a("./indicePrueba", 0);
	string preg;
	double kk1; double kb;

	ASSERT_EQUAL(0,a.DevolverTipoStemming());
	ASSERT_EQUAL(2,a.DevolverParametrosDFR());

	a.DevolverParametrosBM25(kk1, kb);
	ASSERT_EQUAL(1.2, kk1);
	ASSERT_EQUAL(0.75, kb);

	ASSERT_EQUAL(0, a.DevolverFormulaSimilitud());
	ASSERT_EQUAL(true, a.CambiarFormulaSimilitud(1));
	ASSERT_EQUAL(1,a.DevolverFormulaSimilitud());
	ASSERT_EQUAL(false,a.CambiarFormulaSimilitud(2));
	ASSERT_EQUAL(1,a.DevolverFormulaSimilitud());

	ASSERT_EQUAL(false,a.DevuelvePregunta(preg));
	ASSERT_EQUAL("",preg);
	ASSERT_EQUAL(true,a.IndexarPregunta("pal7"));
	ASSERT_EQUAL(true,a.DevuelvePregunta(preg));
	ASSERT_EQUAL("pal7",preg);
}

//LAS SOLUCIONES DEL PROFESOR NO ERAN CORRECTAS, HE PUESTO LAS MÍAS
void buscador02()
{
	IndexadorHash b("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, false);

	ASSERT_EQUAL(true,b.Indexar("./listaFicheros_corto.txt"));
	ASSERT_EQUAL(true,b.GuardarIndexacion());

	Buscador a("./indicePrueba", 0);
	string preg;

	ASSERT_EQUAL(true,a.IndexarPregunta("pal1 pal7"));

	ASSERT_EQUAL(true,a.Buscar(1));

	///Crear un stream para captar lo imprimido por salida estándar
	std::stringstream buffer;
	//Salvar el buffer de cout
	std::streambuf *sbuf = std::cout.rdbuf();
	// Redirigir cout hacia el buffer de stringstream
	std::cout.rdbuf(buffer.rdbuf());

	a.ImprimirResultadoBusqueda(1);

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	string sal= buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();
	string esperado = "0 DFR fichero1 0 1.5 pal1 pal7\n";

	ASSERT_EQUAL(esperado,sal);

	ASSERT_EQUAL(true,a.CambiarFormulaSimilitud(1));

	ASSERT_EQUAL(true,a.Buscar(1));

	//Redirigir la salida estándar
	std::cout.rdbuf(buffer.rdbuf());

	a.ImprimirResultadoBusqueda(1);

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	sal= buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();
	esperado = "0 BM25 fichero1 0 0 pal1 pal7\n";

	ASSERT_EQUAL(esperado,sal);

}

}

namespace felixem
{
	//Tad recogido del pdf pero sin solución del profesor
	void buscador01()
	{
		IndexadorHash b("./StopWordsEspanyol.txt", ". 	,:", false, false,
				"./indicePrueba", 0, false, false);

		ASSERT_EQUAL(true,b.Indexar("./listaFicheros_corto.txt"));
		ASSERT_EQUAL(true,b.GuardarIndexacion());

		Buscador a("./indicePrueba", 0);

		///Crear un stream para captar lo imprimido por salida estándar
		std::stringstream buffer;
		//Salvar el buffer de cout
		std::streambuf *sbuf = std::cout.rdbuf();
		// Redirigir cout hacia el buffer de stringstream
		std::cout.rdbuf(buffer.rdbuf());

		cout << a;

		//Crear stream de salida
		stringstream bufEsperado;
		bufEsperado << "Buscador: \n"
				<< "\tNo hay ninguna pregunta indexada\n"
				<< "\tDatos del indexador: \n"
				<< "Fichero con el listado de palabras de parada: ./StopWordsEspanyol.txt\n"
				<< "Tokenizador: DELIMITADORES: . 	,: TRATA CASOS ESPECIALES: 0 PASAR A MINUSCULAS Y SIN ACENTOS: 0\n"
				<< "Directorio donde se almacenara el indice generado: ./indicePrueba\n"
				<< "Stemmer utilizado: 0\n"
				<<"Informacion de la coleccion indexada: "
				<<"numDocs: 2\tnumTotalPal: 11\tnumTotalPalSinParada: 7\tnumTotalPalDiferentes: 4\ttamBytes: 53\n"
				<<"Se almacenara parte del indice en disco duro: 0\nSe almacenaran las posiciones de los terminos: 0\n";

		//Redirigir buffer de salida a cout
		cout.rdbuf(sbuf);

		string esperado = bufEsperado.str();
		string salida = buffer.str();

		//Limpiar buffer
		buffer.clear();
		buffer.str("");

		ASSERT_EQUAL(esperado,salida);

		/* Estaría accediendo al método de la clase "IndexadorHash" de la
		que "Buscador" está heredando*/
		ASSERT_EQUAL(0,a.DevolverTipoStemming());

		ASSERT_EQUAL(true,a.IndexarPregunta("documentos sobre la Guerra Civil española"));

		ASSERT_EQUAL(true,a.Buscar(20));

		//Cambiar buffer de salida
		cout.rdbuf(buffer.rdbuf());

		a.ImprimirResultadoBusqueda(10);

		//Recuperar salida
		salida = buffer.str();
		cout.rdbuf(sbuf);
		buffer.clear();
		buffer.str("");

		//Establecer salida esperada
		bufEsperado.clear();
		bufEsperado.str("");

		bufEsperado<<"0 DFR fichero1 0 0 documentos sobre la Guerra Civil española\n"
				<<"0 DFR fichero2 1 0 documentos sobre la Guerra Civil española\n";

		esperado = bufEsperado.str();

		ASSERT_EQUAL(esperado,salida);

		ASSERT_EQUAL(true,a.CambiarFormulaSimilitud(1));
		ASSERT_EQUAL(true,a.Buscar(20));

		//Redirigir buffer
		cout.rdbuf(buffer.rdbuf());

		a.ImprimirResultadoBusqueda(10);

		//Recuperar salida
		salida = buffer.str();
		//Redirigir buffer
		cout.rdbuf(sbuf);
		//Limpiar buffers
		buffer.clear();
		buffer.str("");
		bufEsperado.clear();
		bufEsperado.str("");

		//Determinar salida esperada
		bufEsperado<<"0 BM25 fichero1 0 0 documentos sobre la Guerra Civil española\n"
				<<"0 BM25 fichero2 1 0 documentos sobre la Guerra Civil española\n";
		esperado = bufEsperado.str();

		ASSERT_EQUAL(esperado,salida);
	}
}

cute::suite make_suite_TestBuscador(){
	cute::suite s;

	//Tests del profesor
	s.push_back(CUTE(Profesor::buscador01));
	s.push_back(CUTE(Profesor::buscador02));

	//Tests de felixem
	s.push_back(CUTE(felixem::buscador01));

	return s;
}



