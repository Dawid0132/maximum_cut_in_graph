# maximum_cut_in_graph - poprawa

## Spis treści
* [Projekt zawiera](#Projekt-zawiera)
* [Opis problemu](#Opis-problemu)
* [Dodanie połączeń w grafie](#Dodawanie-połączeń-w-grafie-do-pliku)

### Projekt zawiera
<ul>
  <li>Własną funkcję celu dla problemu maximum_cut</li>
  <li>Metodę dla bliskiego sąsiedztwa bieżącego rozwiązania</li>
  <li>Funkcję która wygeneruje losowe rozwiązanie</li>
  <li>Algorytm pełnego przeglądu</li>
  <li>Algorytm wspinaczkowy z wyborem najlepszego sąsiada</li>
  <li>Algorytm tabu</li>
  <li>Algorytm symulowanego wyżarzania</li>
  <li>Algorytm genetyczny</li>
</ul>

### Opis problemu
Bazując na https://en.wikipedia.org/wiki/Maximum_cut udało mi się rozwiązać problem poprzez podzielenie wszystkich możliwych wierzchołków na dwie grupy i sprawdzeniu czy wierzchołek z pierwszej grupy ma połączenie z drugim w ostatniej grupie. Jeśli tak, uznawałem to za przecięcie i przeszukując wszytkie możliwe możliwości znajduje w taki sposób największe rozwiązanie


### Dodawanie połączeń w grafie do pliku
Należy dodawać jedną liczbę na linijkę zaczynając od 0 i jeżeli chcemy dodać wierzchołek to zawsze dodajemy 1 do ostatnio istniejącego w pliku edges.txt . Następnie pierwsza linijka oznacza wierzchołek a druga kolejny który się z nim łączy. Każdy wierzchołek musi mieć conajmniej jedno połączenie. W pliku został umieszczony przykład dla 6 wierzchołków i 7 połączeń w grafie.
