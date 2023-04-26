﻿#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;
// Definicja struktury reprezentującej graf jako macierz sąsiedztwa
struct Graph {
    int V; // Liczba wierzchołków
    bool** adjacencyMatrix; // Macierz sąsiedztwa

    Graph(int V) : V(V) {
        adjacencyMatrix = new bool* [V];
        for (int i = 0; i < V; ++i) {
            adjacencyMatrix[i] = new bool[V];
            for (int j = 0; j < V; ++j) {
                adjacencyMatrix[i][j] = false;
            }
        }
    }

    void addEdge(int v, int w) {
        adjacencyMatrix[v][w] = true;
        adjacencyMatrix[w][v] = true;
    }
};

// Klasa reprezentująca rozwiązanie - kolorowanie grafu
class Solution {
public:
    Solution(int numVertices, int numColors) : numVertices(numVertices), numColors(numColors) {
        // Inicjalizacja kolorów dla wierzchołków
        colors.resize(numVertices, 0);
        for (int i = 0; i < numVertices; ++i) {
            colors[i] = rand() % numColors;
        }
    }

    // Oblicza koszt rozwiązania - liczbę konfliktów kolorów w grafie
    int getCost(const Graph& graph) const {
        int cost = 0;
        for (int i = 0; i < graph.V; ++i) {
            for (int j = i + 1; j < graph.V; ++j) {
                if (graph.adjacencyMatrix[i][j] && colors[i] == colors[j]) {
                    ++cost;
                }
            }
        }
        return cost;
    }

    // Krzyżowanie dwóch rozwiązań
    Solution crossover(const Solution& other) const {
        int crossoverPoint = rand() % numVertices;
        Solution child(numVertices, numColors);
        for (int i = 0; i < crossoverPoint; ++i) {
            child.colors[i] = colors[i];
        }
        for (int i = crossoverPoint; i < numVertices; ++i) {
            child.colors[i] = other.colors[i];
        }
        return child;
    }

    // Mutacja rozwiązania - zamiana koloru w jednym z wierzchołków
    void mutate() {
        int vertex = rand() % numVertices;
        int newColor;
        do {
            newColor = rand() % numColors;
        } while (newColor == colors[vertex]);
        colors[vertex] = newColor;
    }

public:
    int numVertices; // Liczba wierzchołków w grafie
    int numColors; // Liczba dostępnych kolorów
    std::vector<int> colors; // Wektor kolorów dla wierzchołków
};

// Implementacja algorytmu genetycznego
Solution geneticAlgorithm(const Graph& graph, int numColors, int populationSize, int numGenerations) {
    srand(time(0));

    // Inicjalizacja populacji początkowej
    std::vector<Solution> population;
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(Solution(graph.V, numColors));
    }

    // Główna pętla algorytmu genetycznego
    for (int generation = 0; generation < numGenerations; ++generation) {
        // Ocena populacji - obliczenie kosztu dla każdego rozwiązania
        for (Solution& solution : population) {
            solution.getCost(graph);
        }

        // Sortowanie populacji według kosztu - rozwiązania o mniejszym koszcie na początku
        std::sort(population.begin(), population.end(), [&graph](const Solution& a, const Solution& b) {
            return a.getCost(graph) < b.getCost(graph);
            });

        // Wybór najlepszego rozwiązania - rozwiązanie o najmniejszym koszcie
        Solution bestSolution = population[0];

        // Krzyżowanie i mutacja populacji
        std::vector<Solution> newPopulation;
        while (newPopulation.size() < populationSize) {
            // Wybór rodziców - turniej dwóch rozwiązań
            Solution parent1 = population[rand() % populationSize];
            Solution parent2 = population[rand() % populationSize];

            // Krzyżowanie rodziców
            Solution child = parent1.crossover(parent2);

            // Mutacja dziecka
            child.mutate();

            // Dodanie dziecka do nowej populacji
            newPopulation.push_back(child);
        }

        // Aktualizacja populacji
        population = std::move(newPopulation);
    }

    // Zwrócenie najlepszego rozwiązania
    return population[0];
}

int main() {
    // Przykładowy graf
    Graph graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    int numColors = 3; // Liczba dostępnych kolorów
    int populationSize = 50; // Rozmiar populacji
    int numGenerations = 100; // Liczba generacji

    // Wywołanie algorytmu genetycznego i otrzymanie najlepszego rozwiązania
    Solution bestSolution = geneticAlgorithm(graph, numColors, populationSize, numGenerations);

    // Wyświetlenie wyniku
    std::cout << "Najlepsze kolorowanie grafu: "<<endl;
    for (int i = 0; i < graph.V; ++i) {
        std::cout << "Wierzcholek " << i << ": Kolor " << bestSolution.colors[i] << endl;
    }
    std::cout << "Koszt: " << bestSolution.getCost(graph) << std::endl;
    //
    return 0;
}