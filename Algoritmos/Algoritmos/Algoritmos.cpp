#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <iomanip>
#include <locale>

using namespace std;
using namespace chrono;

// Estructura para almacenar resultados
struct Resultado 
{
    string metodo;
    double tiempo;
    string complejidad;
    string observaciones;
};

// Función para generar datos aleatorios
vector<int> generarDatosAleatorios(int cantidad, int minimo, int maximo) 
{
    random_device dispositivo;
    mt19937 generador(dispositivo());
    uniform_int_distribution<> distribucion(minimo, maximo);
    vector<int> datos(cantidad);
    for (int& dato : datos) 
    {
        dato = distribucion(generador);
    }
    return datos;
}


// Ordenamiento por selección
void OrdenamientoSeleccion(vector<int>& arreglo)
{
    int tamano = arreglo.size();
    for (int i = 0; i < tamano - 1; i++) 
    {
        int indice_minimo = i;
        for (int j = i + 1; j < tamano; j++)
        {
            if (arreglo[j] < arreglo[indice_minimo])
            {
                indice_minimo = j;
            }
        }
        swap(arreglo[i], arreglo[indice_minimo]);
    }
}

// Ordenamiento por burbuja
void OrdenamientoBurbuja(vector<int>& arreglo) 
{
    int tamano = arreglo.size();
    for (int i = 0; i < tamano - 1; i++) {
        for (int j = 0; j < tamano - i - 1; j++)
        {
            if (arreglo[j] > arreglo[j + 1]) 
            {
                swap(arreglo[j], arreglo[j + 1]);
            }
        }
    }
}

// Ordenamiento por inserción
void OrdenamientoInsercion(vector<int>& arreglo)
{
    int tamano = arreglo.size();
    for (int i = 1; i < tamano; i++)
    {
        int clave = arreglo[i];
        int j = i - 1;
        while (j >= 0 && arreglo[j] > clave)
        {
            arreglo[j + 1] = arreglo[j];
            j--;
        }
        arreglo[j + 1] = clave;
    }
}

// Ordenamiento ShellSort
void OrdenamientoShellSort(vector<int>& arreglo) 
{
    int tamano = arreglo.size();
    for (int gap = tamano / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < tamano; i++)
        {
            int temp = arreglo[i];
            int j;
            for (j = i; j >= gap && arreglo[j - gap] > temp; j -= gap)
            {
                arreglo[j] = arreglo[j - gap];
            }
            arreglo[j] = temp;
        }
    }
}

// Ordenamiento Radix
void OrdenamientoRadix(vector<int>& arreglo) 
{
    int elemento_maximo = *max_element(arreglo.begin(), arreglo.end());
    for (int exp = 1; elemento_maximo / exp > 0; exp *= 10)
    {
        vector<int> salida(arreglo.size());
        int conteo[10] = { 0 };

        for (int numero : arreglo) 
        {
            conteo[(numero / exp) % 10]++;
        }

        for (int i = 1; i < 10; i++)
        {
            conteo[i] += conteo[i - 1];
        }

        for (int i = arreglo.size() - 1; i >= 0; i--)
        {
            salida[--conteo[(arreglo[i] / exp) % 10]] = arreglo[i];
        }

        arreglo = salida;
    }
}

// Ordenamiento por mezcla
void ordenamientoMezcla(vector<int>& arreglo, int inicio, int fin)
{
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;

        ordenamientoMezcla(arreglo, inicio, medio);
        ordenamientoMezcla(arreglo, medio + 1, fin);

        vector<int> izquierda(arreglo.begin() + inicio, arreglo.begin() + medio + 1);
        vector<int> derecha(arreglo.begin() + medio + 1, arreglo.begin() + fin + 1);

        int i = 0, j = 0, k = inicio;
        while (i < izquierda.size() && j < derecha.size()) 
        {
            if (izquierda[i] <= derecha[j])
            {
                arreglo[k++] = izquierda[i++];
            }
            else
            {
                arreglo[k++] = derecha[j++];
            }
        }

        while (i < izquierda.size()) {
            arreglo[k++] = izquierda[i++];
        }
        while (j < derecha.size()) {
            arreglo[k++] = derecha[j++];
        }
    }
}

// Ordenamiento Quicksort
void quicksort(vector<int>& arreglo, int inicio, int fin)
{
    if (inicio < fin) 
    {
        int pivote = arreglo[fin];
        int indice_menor = inicio - 1;

        for (int j = inicio; j < fin; j++)
        {
            if (arreglo[j] < pivote)
            {
                swap(arreglo[++indice_menor], arreglo[j]);
            }
        }
        swap(arreglo[indice_menor + 1], arreglo[fin]);
        int posicion_pivote = indice_menor + 1;

        quicksort(arreglo, inicio, posicion_pivote - 1);
        quicksort(arreglo, posicion_pivote + 1, fin);
    }
}

void exportarResultadosCSV(const vector<Resultado>& resultados, const string& archivo) 
{
    ofstream salida(archivo);
    if (salida.is_open())
    {
        salida << "Metodo,Tiempo (s),Complejidad,Observaciones\n";
        for (const auto& resultado : resultados)
        {
            salida << resultado.metodo << ","
                << fixed << setprecision(6) << resultado.tiempo << ","
                << resultado.complejidad << ","
                << resultado.observaciones << "\n";
        }
        salida.close();
        cout << "Resultados exportados a " << archivo << endl;
    }
    else 
    {
        cerr << "Error al abrir el archivo " << archivo << endl;
    }
}

void compararTiempos()
{
    const int TAMANO = 10000; // Tamaño del arreglo
    vector<int> datos = generarDatosAleatorios(TAMANO, 0, 100000);
    vector<Resultado> resultados;

    // Métodos con sus respectivas complejidades
    vector<pair<string, void(*)(vector<int>&)>> metodos =
    {
        {"Seleccion", OrdenamientoSeleccion},
        {"Burbuja", OrdenamientoBurbuja},
        {"Insercion", OrdenamientoInsercion},
        {"ShellSort", OrdenamientoShellSort},
        {"Radix", OrdenamientoRadix}
    };

    // Comparar métodos
    for (auto& metodo : metodos) 
    {
        string nombre = metodo.first;
        auto funcion = metodo.second;

        vector<int> copia = datos;
        auto inicio = high_resolution_clock::now();
        funcion(copia);
        auto fin = high_resolution_clock::now();
        duration<double> duracion = fin - inicio;

        resultados.push_back({ nombre, duracion.count(),
            (nombre == "Radix" ? "O(nk)" : "O(n²)"),
            (nombre == "Radix" ? "Rápido para enteros" : "Lento para datos grandes") });
    }

    // Ordenamiento por mezcla
    {
        vector<int> copia = datos;
        auto inicio = high_resolution_clock::now();
        ordenamientoMezcla(copia, 0, copia.size() - 1);
        auto fin = high_resolution_clock::now();
        resultados.push_back({ "Mezcla", duration<double>(fin - inicio).count(),
            "O(n log n)", "Eficiente para cualquier entrada" });
    }

    // Quicksort
    {
        vector<int> copia = datos;
        auto inicio = high_resolution_clock::now();
        quicksort(copia, 0, copia.size() - 1);
        auto fin = high_resolution_clock::now();
        resultados.push_back({ "Quicksort", duration<double>(fin - inicio).count(),
            "O(n log n)", "El mejor rendimiento promedio" });
    }

    // Mostrar resultados en consola
    cout << left << setw(12) << "Metodo" << setw(15) << "Tiempo (s)"
        << setw(15) << "Complejidad" << "Observaciones" << endl;
    for (const auto& resultado : resultados) {
        cout << left << setw(12) << resultado.metodo
            << setw(15) << fixed << setprecision(6) << resultado.tiempo
            << setw(15) << resultado.complejidad
            << resultado.observaciones << endl;
    }

    // Exportar resultados
    exportarResultadosCSV(resultados, "resultados.csv");
}

int main()
{
    setlocale(LC_CTYPE, "es_ES.UTF-8");
    compararTiempos();
    return 0;
}