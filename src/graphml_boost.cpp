#include "graphml_boost.h"


class VisitanteBFS : public boost::default_bfs_visitor
{
    public:
      //This is invoked when a vertex is encountered for the first time.
      void discover_vertex(graphml_boost::vertex_descriptor_t v, const graphml_boost::grafo_t &g) const
      {
        //cout << v << endl;
        return;
      }

      //This is invoked on every vertex of the graph before the start of the graph search.
      void initialize_vertex(graphml_boost::vertex_descriptor_t v, const graphml_boost::grafo_t &g) const { }

      //This is invoked on a vertex as it is popped from the queue. This happens immediately before examine_edge() is invoked on each of the out-edges of vertex u.
      void examine_vertex(graphml_boost::vertex_descriptor_t v, const graphml_boost::grafo_t &g) const { }

      //This is invoked on every out-edge of each vertex after it is discovered.
      void examine_edge(graphml_boost::edge_descriptor_t e,  const graphml_boost::grafo_t &g) const { }

      //This is invoked on each edge as it becomes a member of the edges that form the search tree.
      void tree_edge(graphml_boost::edge_descriptor_t e, const graphml_boost::grafo_t &g) const { }

      //This is invoked on back or cross edges for directed graphs and cross edges for undirected graphs.
      void non_tree_edge(graphml_boost::edge_descriptor_t e, const graphml_boost::grafo_t &g) const {}

      //This is invoked on the subset of non-tree edges whose target vertex is colored gray at the time of examination. The color gray indicates that the vertex is currently in the queue.
      void gray_target(graphml_boost::edge_descriptor_t e, const graphml_boost::grafo_t &g) const {}

      //This is invoked on the subset of non-tree edges whose target vertex is colored black at the time of examination. The color black indicates that the vertex has been removed from the queue.
      void black_target(graphml_boost::edge_descriptor_t e, const graphml_boost::grafo_t &g) const {}

      //This invoked on a vertex after all of its out edges have been added to the search tree and all of the adjacent vertices have been discovered
      //(but before the out-edges of the adjacent vertices have been examined).
      void finish_vertex(graphml_boost::vertex_descriptor_t v, const graphml_boost::grafo_t &g) const { }

};

graphml_boost::graphml_boost()
{
    //ctor
}

graphml_boost::~graphml_boost()
{
    //dtor
    if (_ifstream.is_open()) // En el destructor nos aseguramos de cerrar el fichero por si acaso.
        _ifstream.close();
}

bool graphml_boost::cargaGrafo(string nombreFichero)
{

    try
    {
        limpiaGrafo();
        if (_ifstream.is_open()) // Si el manejador del fichero estaba abierto lo cerramos
            _ifstream.close();

        _ifstream.open(nombreFichero.c_str(), std::ifstream::in); // Abrimos el fichero para pasárselo a read_graphml

        // Configuración de las propiedades de los nodos y las aristas.
        // Coincide exactamente con la definición del archivo xml con el grafo en formato graphml.
        _dp.property("x",boost::get(&nodo_props::x,_grafo));
        _dp.property("y",boost::get(&nodo_props::y,_grafo));
        _dp.property("z",boost::get(&nodo_props::z,_grafo));
        _dp.property("id",boost::get(&nodo_props::id,_grafo));
        _dp.property("type",boost::get(&nodo_props::type,_grafo));        
        _dp.property("idarista",boost::get(&arista_props::id,_grafo));
        _dp.property("source",boost::get(&arista_props::source,_grafo));
        _dp.property("target",boost::get(&arista_props::target,_grafo));
        _dp.property("weight",boost::get(&arista_props::weight,_grafo));

        boost::read_graphml(_ifstream, _grafo, _dp);  // Procedemos a la lectura del archivo y lo cargamos en nuestro grafo
        cout << "Nº de nodos " << boost::num_vertices(_grafo) << "\n";
        cout << "Nº de aristas " << boost::num_edges(_grafo) << "\n";

    }
    catch (std::exception &e)
    {
        cout << e.what();
        return false;
    }

    return true;

}


template <class ParentDecorator>
struct print_parent {
  print_parent(const ParentDecorator& p_) : p(p_) { }
  template <class Vertex>
  void operator()(const Vertex& v) const {
    std::cout << "parent[" << v << "] = " <<  p[v]  << std::endl;
  }
  ParentDecorator p;
};

template <class NewGraph, class Tag>
struct graph_copier
  : public boost::base_visitor<graph_copier<NewGraph, Tag> >
{
  typedef Tag event_filter;

  graph_copier(NewGraph& graph) : new_g(graph) { }

  template <class Edge, class Graph>
  void operator()(Edge e, Graph& g) {
    boost::add_edge(boost::source(e, g), boost::target(e, g), new_g);
  }
private:
  NewGraph& new_g;
};

template <class NewGraph, class Tag>
inline graph_copier<NewGraph, Tag>
copy_graph(NewGraph& g, Tag) {
  return graph_copier<NewGraph, Tag>(g);
}

void graphml_boost::rutaMasCortaBFS(int idNodoOrigen)
{
    typedef std::vector<vertex_descriptor_t>::value_type* Piter; // Solo para pintar datos
    boost::graph_traits<grafo_t>::vertices_size_type d[boost::num_vertices(_grafo)];  // Para almacenar distancias.
    std::fill_n(d, boost::num_vertices(_grafo), 0); // Inicializa el array con ceros. MUY IMPORTANTE, OJO.
                                                    // boost::record_distances no asigna, incrementa el valor
                                                    // por lo que si no lo incializamos a cero habrá, desde primera
                                                    // hora, valores erróneos.


    //std::vector<vertex_descriptor_t> p(boost::num_vertices(_grafo)); // para almacenar las rutas posibles
    //vertex_descriptor_t s = *(boost::vertices(_grafo).first);   // para indicar cual es el nodo de partida HAY QUE MODIFICARLO PARA QUE SEA UN PARAMETRO


    _p.clear();
    _p = std::vector<vertex_descriptor_t>(boost::num_vertices(_grafo));
    //p[s] = s;
    _p[idNodoOrigen] = idNodoOrigen;

    //Llamada al algoritmo BFS, le pasamos:
    // _grafo: el grafo en cuestión.
    // idNodoOrigen: id del nodo desde el que empezar a buscar.
    // Estructura visitor para que el algoritmo vaya guardando distancias y rutas y esto tiene su miga. Todo este parámetro
    // se envuelve con boost::visitor() que es un wrapper para que el algoritmo se lo trague pues lo que espera es precisamente eso, un visitor.
    // Para crear el visitor llamamos a make_bfs_visitor() que es un adaptador al tipo visitor que espera BFS. Como queremos dos visitors para que se almacenen
    // tanto las distancias como las rutas pues se le pasa un pair con ambos visitors, una para distancias y otro para rutas.
    // Qué arte!! Abstracto, eso sí :D
    boost::breadth_first_search(_grafo, idNodoOrigen,//s,
                                boost::visitor(boost::make_bfs_visitor
                                                       (std::make_pair(boost::record_distances(d, boost::on_tree_edge()),
                                                                       boost::record_predecessors(&_p[0],boost::on_tree_edge())))));



    boost::print_graph(_grafo); // Pinto el grafo pq mola

    std::cout << "distances: ";
    for (size_t i=0; i<num_vertices(_grafo);i++)
        cout << d[i] << " ";
    cout << endl;


    // boost::vertices devuelve un pair, o sea, un tipo de dato formado por una pareja de datos. Esa pareja de datos
    // son dos iteradores del tipo boost::adjacency_list::vertex_iterator. Uno apunta al principio (vertices.first)
    // y el otro al final (vertices.second) de los vertices del grafo (al fin y al cabo es un vector (o una lista)).
    // Así pues la siguiente instrucción podría leerse como: para cada vértice desde el primero al último
    // llamo a print_parent que me pinta la info en la consola.
    // Ahora bien, cuando usamos el visitor boost::record_predecessors a la hora de hacer un breadth_first_search, la
    // información de la ruta más corta no se guarda de forma evidente, de alguna manera podríamos decir que comprime
    // la información (si es que no era ya lo bastante complicado :( ).
    // Para saber el primer vértice al que hay que "viajar" miramos en la posición del vector correspondiente al nodo de destino
    // y el contenido será el siguiente vértice al que tenemos que llegar y a su vez ese valor nos servirá para saber cual
    // es el siguiente elemento del vector que tenemos que mirar para saber cual es el siguiente vértice al que llegar. Así hasta llegar
    // a la posición 0 del vector que aloja el indice del nodo de salida. Es decir, hay que recorrer el vector del revés para hacer
    // el recorrido en el orden que realmente queremos. Ole ole, ahora es cuando el entendimiento se rinde y se echa un trago (Terry Pratchet - Mundodisco).
    std::for_each(boost::vertices(_grafo).first, boost::vertices(_grafo).second,
                  print_parent<Piter>(&_p[0]));

    return;

}

inline void graphml_boost::limpiaGrafo()
{
    _grafo.clear();
}

void graphml_boost::rutaMasCortaDijkstra(int idNodoOrigen)
{
    typedef std::vector<vertex_descriptor_t>::value_type* Piter; // Solo para pintar datos
//  std::vector<vertex_descriptor_t> p(boost::num_vertices(_grafo));

    _p.clear();
    _p = std::vector<vertex_descriptor_t>(boost::num_vertices(_grafo));

//  p[s] = s;
    _p[idNodoOrigen] = idNodoOrigen;

    boost::dijkstra_shortest_paths(_grafo, idNodoOrigen, //s,
                                   boost::predecessor_map(&_p[0]).weight_map(boost::get(&arista_props::weight, _grafo)));

    std::for_each(boost::vertices(_grafo).first, boost::vertices(_grafo).second,
                  print_parent<Piter>(&_p[0]));

    return;

}

void graphml_boost::rutasAleatoriasRST(bool conPesos, int idNodoOrigen) //Random_spanning_tree
{
    typedef std::vector<vertex_descriptor_t>::value_type* Piter; // Solo para pintar datos
    //std::vector<vertex_descriptor_t> p(boost::num_vertices(_grafo));
    //vertex_descriptor_t s = *(boost::vertices(_grafo).first);

    _p.clear();
    _p = std::vector<vertex_descriptor_t>(boost::num_vertices(_grafo));

    //p[s]=s;
    _p[idNodoOrigen] = idNodoOrigen;

    std::mt19937 gen{static_cast<uint32_t>(std::time(0))}; // esto es para generar aleatoriedad.

    if (conPesos) //con pesos
        boost::random_spanning_tree(_grafo,gen,boost::predecessor_map(&_p[0]).root_vertex(idNodoOrigen).weight_map(boost::get(&arista_props::weight, _grafo)));
    else  //sin pesos
        boost::random_spanning_tree(_grafo,gen,boost::predecessor_map(&_p[0]).root_vertex(idNodoOrigen));

    std::for_each(boost::vertices(_grafo).first, boost::vertices(_grafo).second,
                  print_parent<Piter>(&_p[0]));

    return;
}


//void graphml_boost::rutasAleatoriasRST(bool conPesos) //Random_spanning_tree
//{
//    typedef std::vector<vertex_descriptor_t>::value_type* Piter; // Solo para pintar datos
//    std::vector<vertex_descriptor_t> p(boost::num_vertices(_grafo));
//    vertex_descriptor_t s = *(boost::vertices(_grafo).first);
//
//    p[s]=s;
//
//    std::mt19937 gen{static_cast<uint32_t>(std::time(0))};
//
//    //boost::random_spanning_tree(_grafo, gen,boost::predecessor_map(&p[0]).root_vertex(destino));
//
//
//
//    if (conPesos) //con pesos
//        boost::random_spanning_tree(_grafo,gen,boost::predecessor_map(&p[0]).root_vertex(s).weight_map(boost::get(&arista_props::weight, _grafo)));
//    else  //sin pesos
//        boost::random_spanning_tree(_grafo,gen,boost::predecessor_map(&p[0]).root_vertex(s));
//
//  std::for_each(boost::vertices(_grafo).first, boost::vertices(_grafo).second,print_parent<Piter>(&p[0]));
//
//  cout << "\nSupongamos que queremos ir del nodo 0 al nodo 8: " << endl;
//  cout << "Hay que pasar por los nodos ";
//  //me la juego, en los grafos de ejemplo que estoy usando fijo que hay mínimo 9 nodos.
//  int i = 8;
//  while (i > 0)
//  {
//     cout << p[i] << ", ";
//     i = p[i];
//  }
//
//
//    return;
//}

/*
 * Get All vertices from one type (pacmanstart,bigpill,regular,phantomZone)
 */
graphml_boost::ruta_t graphml_boost::getVertices(std::string type){

    graphml_boost::ruta_t vec;

    for(uint i=0; i<boost::num_vertices(_grafo); i++)
    {

      if(_grafo[i].type == type){
        _grafo[i].idBoost=i;
        vec.push_back(_grafo[i]);
      }
    }


    return vec;



}
graphml_boost::ruta_t graphml_boost::getRuta(size_t idOrigen, size_t idDestino)
{
    graphml_boost::ruta_t vec;

    if (idDestino < boost::num_vertices(_grafo) && idDestino >= 0) // Aquí podríamos hacer algo más sofisticado como lanzar excepciones y esas cosas :D
    {
        size_t i = idDestino;
        vec.push_back(_grafo[idDestino]);
        while (i != idOrigen && i >= 0)
        {
            vec.push_back(_grafo[_p[i]]);
            i = static_cast<size_t>(_p[i]);
        }
    }

//    for (int j=0; j<vec.size();j++)
//        cout << "vec["<< j << "]" << vec[j].id << endl;

    return vec;

}

graphml_boost::nodo_props graphml_boost::getNodoAleatorio()
{
  graphml_boost::nodo_props nodo{"0","0","0","0","0",-1};
    if (boost::num_vertices(_grafo))
    {
        std::random_device rd;      //non-deterministic uniform random number generator COMO MOLA :D
        std::mt19937 gen(rd());     //Algoritmo generador: 32-bit Mersenne Twister by Matsumoto and Nishimura, 1998
        std::uniform_int_distribution<> dis(0,boost::num_vertices(_grafo)-1); // post-processes the output of an random number engine
                                                                              // in such a way that resulting output is distributed according to a defined
                                                                              // statistical probability density function. Distribuye números en una sucesión dentro de un rango dado.
        nodo = _grafo[dis(gen)];        // Y ahora cogemos uno de los numeros dentro de la distribución y lo usamos como indice para recuperar un nodo del grafo. MOLONGO DE LA MUERTE.
    }

    return nodo;

}
