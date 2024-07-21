# maximum_cut_in_graph - poprawa

## Spis treści
* [Opis problemu](#Opis-problemu)
* [Dodanie połączeń w grafie](#Dodawanie-połączeń-w-grafie-do-edges.txt)


### Opis problemu
Bazując na https://en.wikipedia.org/wiki/Maximum_cut udało mi się rozwiązać problem poprzez podzielenie wszystkich możliwych wierzchołków na dwie grupy i sprawdzeniu czy wierzchołek z pierwszej grupy ma połączenie z drugim w ostatniej grupie jeśli tak uznawałem to za przecięcie i przeszukując wszytkie możliwe możliwości znajduje w taki sposób największe rozwiązanie


### Dodawanie połączeń w grafie do pliku edges.txt
Należy dodawać jedną liczbę na linijkę zaczynając od 0 i jeżeli chcemy dodać wierzchołek to zawsze dodajemy 1 do ostatnio istniejącego w pliku. Następnie pierwsza linijka oznacza wierzchołek a druga kolejny który się z nim łączy. Każdy wierzchołek musi mieć conajmniej jedno połączenie. W pliku został umieszczony przykład dla 6 wierzchołków i 7 połączeń w grafie.
