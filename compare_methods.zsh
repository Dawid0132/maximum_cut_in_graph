#!/bin/zsh

SOURCE_FILE="main.cpp"

EXECUTABLE="metaheuristic_project"

methods=("Własna metoda" "Metoda losowości" "Algorytm pełnego przeglądu" "Algorytm wspinaczkowy najlepszy sąsiad" "Algorytm wspinaczkowy losowy sąsiad" "Algorytm tabu" "Algorytm symulowanego wyżarzania" "Algorytm genetyczny")


choose_method() {
  choose=true

  while $choose; do
      echo "Wybierz metodę do skompilowania"

      for (( i = 1; i <= $#methods; i++ )); do
          echo -e "[$i] ${methods[i]}"
      done

      read "method_choice?Wprowadź pełną nazwę metody: "




        for (( i = 0; i <= $#methods; i++ )); do
          if [ "$method_choice" = "${methods[i]}" ]; then
              method_found=true
              choose=false
              echo -e " "
              echo -e "****** ${methods[i]} ******"
              echo -e " "
              ./"$EXECUTABLE" "${methods[i]}"
              break
          fi
      done


      if [ ! $method_found ]; then
          echo -e "Nie ma takiej metody. Spróbuj ponownie"
      fi

      sleep 2

  done
}

choose_method