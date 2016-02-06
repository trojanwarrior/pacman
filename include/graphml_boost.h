#ifndef GRAPHML_BOOST_H
#define GRAPHML_BOOST_H

#include <fstream>
#include <iostream>
#include <stack>
#include <boost/config.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/random_spanning_tree.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/graph_utility.hpp>

/*
#include <array>
#include <utility>
#include <random>
#include <iostream>
#include <ctime>
#include <cstdint>
*/

using namespace std;

class graphml_boost
{
   public:
    // Definición de tipos y estructuras necesarias
    struct nodo_props      // Propiedades de un nodo (a.k.a los datos que realmente nos interesa albergar en un nodo.
    {
      std::string id;            // id del nodo, externo al grafo, es decir este lo definimos nosotros. boost_graph tiene su propio índice.
      int idBoost;
      std::string x,y,z; // coordenadas físicas en el espacio virtual de un nodo.
      bool interseccion; // si en este nodo sus aristas salientes (o entrantes según se mire) son perpendiculares. Implica un posible
      std::string type;
                           // cambio de dirección del fantasma que se encuentre en ese nodo. Es útil para el fantasma bobo, de modo que
                           // al llegar a una esquina se volviera a calcular su ruta de movimiento.
    };

    typedef nodo_props nodo_props_t;
    typedef std::vector<nodo_props_t> ruta_t; // Tipo vector de propiedades de un nodo. Para declarar un vector que albergue la ruta elegida de las posibles
                                              // devueltas por los algoritmos de búsqueda.

    struct arista_props // Propiedades de una arista (a.k.a. los datos que nos interesa albergar de una arista.
    {
        std::string id; // id de la arista, externo al grafo.
        int source;     // nodo origen de la arista.
        int target;     // nodo destino de la arista.
        int weight;     // peso para cáculo de rutas que lo tenga en cuenta.
    };

    struct graph_props      // Propiedades del grafo. Puede ser útil para saber que nivel de pacman estamos manejando.
    {
        int id;             // Identificador del grafo. Puede ser útil si en un futuro escalamos esta clase para manejar más de un grafo.
        std::string nombre; // Nombre del grafo/nivel de pacman. P.ej. level1
    };

    typedef boost::adjacency_list<
            boost::vecS,        // OutEdgeList: estructura de datos para las aristas
            boost::vecS,        // VertexList: estructura de datos para los vertices (nodos del grafo)
            boost::undirectedS, // directedS (grafo dirigido) o undirectedS (grafo no dirigido). En el pacman será no dirigido pues nos podemos mover en todas direcciones.
            nodo_props,         // VertexProperties (atributos de un vertice/nodo). boost::no_property o un struct que modele los datos de un nodo
            arista_props,       // EdgeProperties (atributos de una arista (p.ej. el peso). boost::no_property o un struct que modele los datos de una arista.
            graph_props         // GraphProperties (atributos comunes a todo el grafo) boost::no_property o un struct que modele los atributos del grafo.
                                // P.ej. para obtener el nombre: (boost::get(graph_properties, _grafo))[_grafo].nombre
    > grafo_t;

    typedef boost::graph_traits<grafo_t>::vertex_descriptor vertex_descriptor_t;  // rasgos (características) de un grafo: para sus nodos/vértices
    typedef boost::graph_traits<grafo_t>::edge_descriptor edge_descriptor_t;      // rasgos (características) de un grafo: para sus aristas/arcos


        graphml_boost();
        virtual ~graphml_boost();

        void limpiaGrafo();                         // hace un clear del grafo dejándolo sin ningún vértice/arista. Para cuando se cargue un nivel nuevo.
        bool cargaGrafo(string nombreFichero);     // Carga del fichero xml con la especificación del grafo
        void rutaMasCortaDijkstra(int idNodoOrigen); // para calcular ruta más corta en grafos con pesos en sus aristas
        void rutaMasCortaBFS(int idNodoOrigen);     // algoritmo breadht_first_search para grafos con peso en las aristas igual a 1 para todas, es decir sin peso.
        void rutasAleatoriasRST(bool conPesos, int idNodoOrigen); // Random_spanning_tree calcula rutas aleatorias, ideal para el fantasma bobo.
        ruta_t getRuta(size_t idOrigen, size_t idDestino); // Devuelve un vector con la lista de nodos (indices del grafo) a visitar. HAY QUE RECORRERLO A LA INVERSA!!!!
                                                           // Cada vez que hagamos una búsqueda, del tipo que sea, necesitaremos llamar a este método para
                                                           // obtener una lista de nodos nueva. El vector que nos devuelva no será válido en el momento que nuestro
                                                           // idOrigen haya cambiado, momento en el que deberemos volver a llamar a los algoritmos de búsqueda.
        ruta_t getVertices(std::string type);



    private:
        string _nombreFichero;          // nombre del fichero graphml con la especificación del grafo.
        std::ifstream _ifstream;        // manejador del fichero.
        boost::dynamic_properties _dp;  // esto sirve para establecer el vínculo entre las propiedades de los vértices/aristas que leemos del fichero,
                                        // es decir, añadimos nuestras propias propiedades al grafo mediante esta estructura. boost_graph ya maneja
                                        // una serie de propiedades (internas) por defecto tales como vertex_index_t
        grafo_t _grafo;                 // Instancia del grafo en cuestión.
        std::vector<vertex_descriptor_t> _p;  //Vector que albergará las rutas desde un nodo dado después de llamar a un algoritmo de busqueda.





};

#endif // GRAPHML_BOOST_H
