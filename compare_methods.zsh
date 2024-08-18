#!/bin/zsh

SOURCE_FILE="main.cpp"

EXECUTABLE="metaheuristic_project"

methods=("Własna metoda" "Metoda losowości" "Algorytm pełnego przeglądu" "Algorytm wspinaczkowy najlepszy sąsiad" "Algorytm wspinaczkowy losowy sąsiad" "Algorytm tabu" "Algorytm symulowanego wyżarzania" "Algorytm genetyczny model tradycyjny" "Algorytm genetyczny model wyspowy")


choose_method() {
  choose=true

  while $choose; do
      echo "Wybierz metodę do skompilowania"

      for (( i = 1; i <= $#methods; i++ )); do
          echo -e "[$i] ${methods[i]}"
      done

      read "method_choice?Wprowadź cyfrę odpowiadającą metodzie: "


        for (( i = 1; i <= $#methods; i++ )); do
          if [ "$method_choice" -eq "$i" ]; then
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
          echo -e "Podałes złą cyfrę. Spróbuj ponownie"
      fi

      sleep 2

  done
}

choose_method