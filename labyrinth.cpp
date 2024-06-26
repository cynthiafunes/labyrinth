#include <iostream> 
#include <vector>   
#include <queue>    
#include <stack>     
#include <cstdlib>   
#include <ctime>     

// Definición de los tipos de celdas en el laberinto
enum Celda { PARED, CLIBRE, VISITADO };  

// Estructura para representar una posición en el laberinto
struct Posicion {  
    int x, y;  
    Posicion(int x, int y) : x(x), y(y) {} 
};

// Declaración del laberinto como una matriz de celdas sin dimensiones inicialmente
std::vector<std::vector<Celda>> laberinto;

void imprimirLaberinto(int ALTO, int ANCHO) {  
    for (int y = 0; y < ALTO; y++) {  
        for (int x = 0; x < ANCHO; x++) {  
            // Imprime cada celda del laberinto con diferentes colores dependiendo de su tipo
            std::cout << (laberinto[y][x] == PARED ? "\033[40m  " : (laberinto[y][x] == CLIBRE ? "\033[47m  " : "\033[42m  "));  
        }
        std::cout << "\033[0m\n";
    }
}

bool dentroDelLaberinto(int x, int y, int ALTO, int ANCHO) {  
    return x >= 0 && y >= 0 && x < ANCHO && y < ALTO; 
}

// Función para generar el laberinto utilizando un algoritmo basado en DFS
void generarLaberinto(int inicioX, int inicioY, int ALTO, int ANCHO) {  
    std::stack<Posicion> pila;  
    pila.push(Posicion(inicioX, inicioY));  
    laberinto[inicioY][inicioX] = CLIBRE; 
    
    while (!pila.empty()) {  
        Posicion actual = pila.top();  
        pila.pop();  

        std::vector<Posicion> vecinos;  
        for (int i : {2, -2}) {  
            if (dentroDelLaberinto(actual.x + i, actual.y, ALTO, ANCHO)) 
                vecinos.push_back(Posicion(actual.x + i, actual.y));  
            if (dentroDelLaberinto(actual.x, actual.y + i, ALTO, ANCHO)) 
                vecinos.push_back(Posicion(actual.x, actual.y + i)); 
        }

        std::vector<Posicion> noVisitados;  
        for (auto vecino : vecinos) {  
            if (laberinto[vecino.y][vecino.x] == PARED) 
                noVisitados.push_back(vecino);  
        }

        if (!noVisitados.empty()) { 
            pila.push(actual);  
            Posicion elegido = noVisitados[rand() % noVisitados.size()];  
            laberinto[(actual.y + elegido.y) / 2][(actual.x + elegido.x) / 2] = CLIBRE;  
            laberinto[elegido.y][elegido.x] = CLIBRE;  
            pila.push(elegido); 
        }
    }
}

// Función para resolver el laberinto utilizando BFS
bool resolverLaberinto(int inicioX, int inicioY, int ALTO, int ANCHO) {  
    std::queue<Posicion> cola;  
    std::vector<std::vector<Posicion>> prev(ALTO, std::vector<Posicion>(ANCHO, Posicion(-1, -1)));  
    cola.push(Posicion(inicioX, inicioY));  

    int movs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}}; 
    
    while (!cola.empty()) {  
        Posicion actual = cola.front();  
        cola.pop();  

        if (actual.x == ANCHO - 2 && actual.y == ALTO - 2) {                                              // Verifica si hemos llegado a la salida
            for (Posicion pos = actual; pos.x != inicioX || pos.y != inicioY; pos = prev[pos.y][pos.x])  // Marca el camino desde la salida hasta el inicio
                laberinto[pos.y][pos.x] = VISITADO;  
            laberinto[inicioY][inicioX] = VISITADO;  
            return true;  
        }
        
        for (auto& m : movs) { 
            int nuevoX = actual.x + m[0], nuevoY = actual.y + m[1];  
            if (dentroDelLaberinto(nuevoX, nuevoY, ALTO, ANCHO) && laberinto[nuevoY][nuevoX] == CLIBRE && prev[nuevoY][nuevoX].x == -1) {  
                cola.push(Posicion(nuevoX, nuevoY)); 
                prev[nuevoY][nuevoX] = actual;  
            }
        }
    }
    return false;  
}

int main() {  
    int ANCHO, ALTO;

    do {
        std::cout << "Ingrese el numero de filas, tiene que ser impar: ";
        std::cin >> ALTO;
        if (ALTO % 2 == 0) {
            std::cout << "Error: El numero de filas debe ser impar.\n";
        }
    } while (ALTO % 2 == 0);

    do {
        std::cout << "Ingrese el numero de columnas, tiene que ser impar: ";
        std::cin >> ANCHO;
        if (ANCHO % 2 == 0) {
            std::cout << "Error: El numero de columnas debe ser impar.\n";
        }
    } while (ANCHO % 2 == 0);

    laberinto.resize(ALTO, std::vector<Celda>(ANCHO, PARED)); 

    srand(time(0));  
    generarLaberinto(1, 1, ALTO, ANCHO);            
    std::cout << "Laberinto Generado:\n";  
    imprimirLaberinto(ALTO, ANCHO);                 
    std::cout << "\nResolviendo laberinto:\n\n";  
    resolverLaberinto(1, 1, ALTO, ANCHO);           
    imprimirLaberinto(ALTO, ANCHO);                 
    return 0;
}
