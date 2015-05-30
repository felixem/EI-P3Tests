#include "TestIndexador.h"
#include "indexadorHash.h"

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

//indezador
void indexador01()
{
	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", true, true, "./indicePrueba", 1, false, true);

	stringstream sal;
	string esperado;

	sal << "Fichero con el listado de palabras de parada: ./StopWordsEspanyol.txt\n" <<
			"Tokenizador: DELIMITADORES: . ,: TRATA CASOS ESPECIALES: 1 PASAR A MINUSCULAS Y SIN ACENTOS: 1\n"<<
			"Directorio donde se almacenara el indice generado: ./indicePrueba\n"<<
			"Stemmer utilizado: 1\n"<<
			"Informacion de la coleccion indexada: numDocs: 0	numTotalPal: 0	numTotalPalSinParada: 0	numTotalPalDiferentes: 0	tamBytes: 0\n"<<
			"Se almacenara parte del indice en disco duro: 0\n"<<
			"Se almacenaran las posiciones de los terminos: 1\n";
	esperado = sal.str();

	stringstream ss1;
	ss1 << a;
	string salida = ss1.str();

	//Comparar mensajes
	ASSERT_EQUAL(esperado, salida);

	IndexadorHash b("./StopWordsIngles.txt", ". ,:", true, false, "./indicePruebaIngles", 2, true, true);

	//Limpiar
	sal.str("");
	sal.clear();

	sal << "Fichero con el listado de palabras de parada: ./StopWordsIngles.txt\n"<<
			"Tokenizador: DELIMITADORES: . ,: TRATA CASOS ESPECIALES: 1 PASAR A MINUSCULAS Y SIN ACENTOS: 0\n"<<
			"Directorio donde se almacenara el indice generado: ./indicePruebaIngles\n"<<
			"Stemmer utilizado: 2\n"<<
			"Informacion de la coleccion indexada: numDocs: 0	numTotalPal: 0	numTotalPalSinParada: 0	numTotalPalDiferentes: 0	tamBytes: 0\n"<<
			"Se almacenara parte del indice en disco duro: 1\n"<<
			"Se almacenaran las posiciones de los terminos: 1\n";

	esperado = sal.str();

	stringstream ss2;
	ss2 << b;
	salida = ss2.str();

	//Comparar mensajes
	ASSERT_EQUAL(esperado,salida);

	//Comprobar getters
	ASSERT_EQUAL(". ,:",b.DevolverDelimitadores ());
	ASSERT_EQUAL("./indicePruebaIngles",b.DevolverDirIndice ());
	ASSERT_EQUAL(2,b.DevolverTipoStemming ());
	ASSERT_EQUAL(1,b.DevolverAlmEnDisco ());
	ASSERT_EQUAL(1,b.DevolverAlmacenarPosTerm ());
	ASSERT_EQUAL(0,b.DevolverPasarAminuscSinAcentos ());
	ASSERT_EQUAL(1,b.DevolverCasosEspeciales ());

}

//indexador02
void indexador02()
{

	IndexadorHash a("./StopWordsEspanyol_corto1.txt", ". ,:", true, true, "./indicePrueba", 1, false, true);

	///Crear un stream para captar lo imprimido por salida estándar
	std::stringstream buffer;
	//Salvar el buffer de cout
	std::streambuf *sbuf = std::cout.rdbuf();
	// Redirigir cout hacia el buffer de stringstream
	std::cout.rdbuf(buffer.rdbuf());

	//Ejecutar acción
	a.ListarPalParada();

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	string salida = buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();

	//Comparar salidas
	ASSERT_EQUAL("ajena\n", salida);
	ASSERT_EQUAL("./StopWordsEspanyol_corto1.txt", a.DevolverFichPalParada ());
	ASSERT_EQUAL(1, a.NumPalParada());

	IndexadorHash b("./StopWordsEspanyol_corto2.txt", ". ,:", true, true, "./indicePrueba", 1, false, true);

	// Redirigir cout hacia el buffer de stringstream
	std::cout.rdbuf(buffer.rdbuf());

	//Ejecutar acción
	b.ListarPalParada();

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	salida = buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();

	//Comparar salidas
	ASSERT_EQUAL("ajena\n", salida);
	ASSERT_EQUAL("./StopWordsEspanyol_corto2.txt", b.DevolverFichPalParada ());
	ASSERT_EQUAL(1, b.NumPalParada());
}

//indexador03
void indexador03()
{
	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	bool salida = a.Indexar("./listaFicheros_corto.txt");
	ASSERT_EQUAL(true,salida);

	int res = a.NumPalIndexadas();
	ASSERT_EQUAL(4,res);

	///Crear un stream para captar lo imprimido por salida estándar
	std::stringstream buffer;
	//Salvar el buffer de cout
	std::streambuf *sbuf = std::cout.rdbuf();
	// Redirigir cout hacia el buffer de stringstream
	std::cout.rdbuf(buffer.rdbuf());

	a.ListarDocs("corpus_corto/fichero1.txt");

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	string sal= buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();

	string esperado = "corpus_corto/fichero1.txt	idDoc: 1	numPal: 6	numPalSinParada: 4	numPalDiferentes: 3	tamBytes: 30\n";
	ASSERT_EQUAL(esperado,sal);

	// Redirigir cout hacia el buffer de stringstream
	std::cout.rdbuf(buffer.rdbuf());

	a.ListarDocs("corpus_corto/fichero2.txt");

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	sal= buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();

	esperado = "corpus_corto/fichero2.txt	idDoc: 2	numPal: 5	numPalSinParada: 3	numPalDiferentes: 2	tamBytes: 23\n";
	ASSERT_EQUAL(esperado,sal);

	salida = a.ListarDocs("corpus_corto/fichero3.txt");
	ASSERT_EQUAL(false,salida);

	// Redirigir cout hacia el buffer de stringstream
	std::cout.rdbuf(buffer.rdbuf());

	a.ListarInfColeccDocs();

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	sal= buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();

	esperado = "numDocs: 2	numTotalPal: 11	numTotalPalSinParada: 7	numTotalPalDiferentes: 4	tamBytes: 53\n";
	ASSERT_EQUAL(esperado,sal);
}

//indexador04
void indexador04()
{
	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	a.Indexar("./listaFicheros_corto.txt");

	InformacionTermino inf1;

	ASSERT_EQUAL(true,a.Devuelve("pal1", inf1));

	string esperado = "Frecuencia total: 2	fd: 1	Id.Doc: 1	ft: 2	0	3";
	stringstream sal;
	sal << inf1;
	string salida = sal.str();

	ASSERT_EQUAL(esperado,salida);
	ASSERT_EQUAL(false,a.Devuelve("pal7", inf1));

	InfTermDoc infDoc1;

	ASSERT_EQUAL(true,a.Devuelve("pal1", "corpus_corto/fichero1.txt", infDoc1));
	ASSERT_EQUAL(false,a.Devuelve("pal1", "fichero1.txt", infDoc1));
	ASSERT_EQUAL(false,a.Devuelve("pal7", "corpus_corto/fichero1.txt", infDoc1));
	ASSERT_EQUAL(true,a.Existe("pal1"));
	ASSERT_EQUAL(false,a.Existe("pal7"));
}

//indexador05
void indexador05()
{
	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	a.Indexar("./listaFicheros_corto.txt");

	ASSERT_EQUAL(true,a.BorraDoc("corpus_corto/fichero1.txt"));

	InformacionTermino inf1;

	ASSERT_EQUAL(true,a.Devuelve("pal2", inf1));

	string esperado = "Frecuencia total: 2	fd: 1	Id.Doc: 2	ft: 2	0	2";
	stringstream sal;
	sal << inf1;
	string salida = sal.str();

	ASSERT_EQUAL(esperado,salida);
	ASSERT_EQUAL(false,a.Devuelve("pal1", inf1));
	ASSERT_EQUAL(2, a.NumPalIndexadas());

	///Crear un stream para captar lo imprimido por salida estándar
	std::stringstream buffer;
	//Salvar el buffer de cout
	std::streambuf *sbuf = std::cout.rdbuf();
	// Redirigir cout hacia el buffer de stringstream
	std::cout.rdbuf(buffer.rdbuf());

	a.ListarDocs("corpus_corto/fichero1.txt");

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	salida= buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();

	ASSERT_EQUAL("",salida);

	// Redirigir cout hacia el buffer de stringstream
	std::cout.rdbuf(buffer.rdbuf());

	a.ListarDocs("corpus_corto/fichero2.txt");

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	salida= buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();

	esperado = "corpus_corto/fichero2.txt	idDoc: 2	numPal: 5	numPalSinParada: 3	numPalDiferentes: 2	tamBytes: 23\n";

	ASSERT_EQUAL(esperado,salida);
	ASSERT_EQUAL(false,a.ListarDocs("corpus_corto/fichero1.txt"));

	// Redirigir cout hacia el buffer de stringstream
	std::cout.rdbuf(buffer.rdbuf());

	a.ListarInfColeccDocs();

	//Redirigir cout a su buffer original
	std::cout.rdbuf(sbuf);
	//Obtener la salida del buffer
	salida= buffer.str();
	//Limpiar el buffer
	buffer.str("");
	buffer.clear();

	esperado = "numDocs: 1	numTotalPal: 5	numTotalPalSinParada: 3	numTotalPalDiferentes: 2	tamBytes: 23\n";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(false,a.BorraDoc("corpus_corto/fichero1.txt"));
	ASSERT_EQUAL(false,a.BorraDoc("corpus_corto/fichero7.txt"));
}

//indexador06
void indexador06()
{
	IndexadorHash b("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	b.Indexar("./listaFicheros_corto.txt");
	b.GuardarIndexacion();

	IndexadorHash a("./indicePrueba");

	ASSERT_EQUAL(". ,:",a.DevolverDelimitadores ());
	ASSERT_EQUAL("./indicePrueba",a.DevolverDirIndice ());
	ASSERT_EQUAL(0,a.DevolverTipoStemming ());
	ASSERT_EQUAL(0,a.DevolverAlmEnDisco ());

	InformacionTermino inf1;

	ASSERT_EQUAL(true, a.Devuelve("pal1", inf1));

	string esperado = "Frecuencia total: 2	fd: 1	Id.Doc: 1	ft: 2	0	3";
	stringstream sal;
	sal << inf1;
	string salida = sal.str();

	ASSERT_EQUAL(esperado,salida);
	ASSERT_EQUAL(false,a.Devuelve("pal7", inf1));

	InfTermDoc infDoc1;

	ASSERT_EQUAL(true,a.Devuelve("pal1", "corpus_corto/fichero1.txt", infDoc1));

	esperado = "ft: 2	0	3";

	//Limpiar salida
	sal.str("");
	sal.clear();

	sal << infDoc1;
	salida = sal.str();

	ASSERT_EQUAL(esperado,salida);
	ASSERT_EQUAL(false,a.Devuelve("pal1", "fichero1.txt", infDoc1));
	ASSERT_EQUAL(false,a.Devuelve("pal7", "corpus_corto/fichero1.txt", infDoc1));
	ASSERT_EQUAL(true,a.Existe("pal1"));
	ASSERT_EQUAL(false,a.Existe("pal7"));
}

//indexador07
void indexador07()
{
	IndexadorHash b("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	b.Indexar("./listaFicheros_corto.txt");

	IndexadorHash a(b);

	ASSERT_EQUAL(". ,:",a.DevolverDelimitadores());
	ASSERT_EQUAL("./indicePrueba",a.DevolverDirIndice());
	ASSERT_EQUAL(0,a.DevolverTipoStemming());
	ASSERT_EQUAL(0,a.DevolverAlmEnDisco());

	InformacionTermino inf1;

	ASSERT_EQUAL(true,a.Devuelve("pal1", inf1));
	stringstream ss;
	ss << inf1;
	string salida = ss.str();
	string esperado = "Frecuencia total: 2	fd: 1	Id.Doc: 1	ft: 2	0	3";
	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(false,a.Devuelve("pal7", inf1));

	InfTermDoc infDoc1;

	ASSERT_EQUAL(true,a.Devuelve("pal1", "corpus_corto/fichero1.txt", infDoc1));

	//Limpiar buffer
	ss.str("");
	ss.clear();

	ss << infDoc1;
	salida = ss.str();
	esperado = "ft: 2	0	3";
	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(false,a.Devuelve("pal1", "fichero1.txt", infDoc1));
	ASSERT_EQUAL(false,a.Devuelve("pal7", "corpus_corto/fichero1.txt", infDoc1));
	ASSERT_EQUAL(true,a.Existe("pal1"));
	ASSERT_EQUAL(false,a.Existe("pal7"));
}

//indexador08
void indexador08()
{
	IndexadorHash b("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	ASSERT_EQUAL(false,b.IndexarDirectorio("./directorio_que_no_existe"));

	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	ASSERT_EQUAL(true,a.IndexarDirectorio("./corpus_corto"));

	InformacionTermino inf1;

	ASSERT_EQUAL(true,a.Devuelve("pal1", inf1));

	stringstream ss;
	ss << inf1;
	string salida = ss.str();
	string esperado = "Frecuencia total: 2	fd: 1	Id.Doc: 1	ft: 2	0	3";
	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(false,a.Devuelve("pal7", inf1));

	InfTermDoc infDoc1;

	ASSERT_EQUAL(true,a.Devuelve("pal1", "./corpus_corto/fichero1.txt", infDoc1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << infDoc1;
	salida = ss.str();
	esperado = "ft: 2	0	3";
	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(false,a.Devuelve("pal1", "fichero1.txt", infDoc1));
	ASSERT_EQUAL(false,a.Devuelve("pal7", "./corpus_corto/fichero1.txt", infDoc1));
	ASSERT_EQUAL(true,a.Existe("pal1"));
	ASSERT_EQUAL(false,a.Existe("pal7"));
}

//indexador09
void indexador09()
{
	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	string preg1;
	InformacionPregunta infPreg1;

	ASSERT_EQUAL(false,a.DevuelvePregunta(preg1));
	ASSERT_EQUAL(false,a.DevuelvePregunta(infPreg1));

	a.IndexarPregunta("pal1 yo pal2 pal1. pal3 el  ");

	ASSERT_EQUAL(true,a.DevuelvePregunta(preg1));

	stringstream ss;
	ss << preg1;
	string salida = ss.str();
	string esperado = "pal1 yo pal2 pal1. pal3 el  ";

	ASSERT_EQUAL(esperado,salida);

	InformacionTerminoPregunta inf1;

	ASSERT_EQUAL(true,a.DevuelvePregunta("pal1", inf1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << inf1;
	salida = ss.str();
	esperado = "ft: 2	0	3";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(false,a.DevuelvePregunta("pal7", inf1));

	ASSERT_EQUAL(true,a.DevuelvePregunta(infPreg1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << infPreg1;
	salida = ss.str();
	esperado = "numTotalPal: 6	numTotalPalSinParada: 4	numTotalPalDiferentes: 3";

	ASSERT_EQUAL(esperado,salida);
}

//indexador10
void indexador10()
{
	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	string preg1;
	InformacionPregunta infPreg1;
	InformacionTerminoPregunta inf1;

	a.IndexarPregunta("pal1 yo pal2 pal1. pal3 el  ");
	a.IndexarPregunta("pal1 yo pal3 el  pal7");

	ASSERT_EQUAL(true,a.DevuelvePregunta(preg1));

	stringstream ss;
	ss<<preg1;
	string salida = ss.str();
	string esperado = "pal1 yo pal3 el  pal7";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(true,a.DevuelvePregunta("pal1", inf1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << inf1;
	salida = ss.str();
	esperado = "ft: 1	0";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(true,a.DevuelvePregunta("pal7", inf1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << inf1;
	salida = ss.str();
	esperado = "ft: 1	4";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(true,a.DevuelvePregunta(infPreg1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << infPreg1;
	salida = ss.str();
	esperado = "numTotalPal: 5	numTotalPalSinParada: 3	numTotalPalDiferentes: 3";
}

//indexador11
void indexador11()
{
	IndexadorHash a("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, true, true);

	a.Indexar("./listaFicheros_corto.txt");

	InformacionTermino inf1;

	ASSERT_EQUAL(true,a.Devuelve("pal1", inf1));

	stringstream ss;
	ss << inf1;
	string salida = ss.str();
	string esperado = "Frecuencia total: 2	fd: 1	Id.Doc: 1	ft: 2	0	3";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(false,a.Devuelve("pal7", inf1));

	InfTermDoc infDoc1;

	ASSERT_EQUAL(true,a.Devuelve("pal1", "corpus_corto/fichero1.txt", infDoc1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << infDoc1;
	salida = ss.str();
	esperado = "ft: 2	0	3";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(false,a.Devuelve("pal1", "fichero1.txt", infDoc1));
	ASSERT_EQUAL(false,a.Devuelve("pal7", "corpus_corto/fichero1.txt", infDoc1));
	ASSERT_EQUAL(true,a.Existe("pal1"));
	ASSERT_EQUAL(false,a.Existe("pal7"));
}

}

namespace felixem
{

//Probar serialización y deserialización completa
void serializacion_pregunta()
{
	IndexadorHash b("./StopWordsEspanyol.txt", ". ,:", false, false, "./indicePrueba", 0, false, true);

	string preg1;
	InformacionPregunta infPreg1;
	InformacionTerminoPregunta inf1;

	b.IndexarPregunta("pal1 yo pal3 el  pal7");
	b.GuardarIndexacion();

	IndexadorHash a("indicePrueba");

	//Cargar indexacion

	ASSERT_EQUAL(true,a.DevuelvePregunta(preg1));

	stringstream ss;
	ss<<preg1;
	string salida = ss.str();
	string esperado = "pal1 yo pal3 el  pal7";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(true,a.DevuelvePregunta("pal1", inf1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << inf1;
	salida = ss.str();
	esperado = "ft: 1	0";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(true,a.DevuelvePregunta("pal7", inf1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << inf1;
	salida = ss.str();
	esperado = "ft: 1	4";

	ASSERT_EQUAL(esperado,salida);

	ASSERT_EQUAL(true,a.DevuelvePregunta(infPreg1));

	//Limpiar
	ss.str("");
	ss.clear();

	ss << infPreg1;
	salida = ss.str();
	esperado = "numTotalPal: 5	numTotalPalSinParada: 3	numTotalPalDiferentes: 3";
}
}

cute::suite make_suite_TestIndexador(){
	cute::suite s;

	//Tests del profesor
	s.push_back(CUTE(Profesor::indexador01));
	s.push_back(CUTE(Profesor::indexador02));
	s.push_back(CUTE(Profesor::indexador03));
	s.push_back(CUTE(Profesor::indexador04));
	s.push_back(CUTE(Profesor::indexador05));
	s.push_back(CUTE(Profesor::indexador06));
	s.push_back(CUTE(Profesor::indexador07));
	s.push_back(CUTE(Profesor::indexador08));
	s.push_back(CUTE(Profesor::indexador09));
	s.push_back(CUTE(Profesor::indexador10));
	s.push_back(CUTE(Profesor::indexador11));

	//Tests de Félix
	s.push_back(CUTE(felixem::serializacion_pregunta));

	return s;
}



