#!/bin/bash

SOURCE_FILE="main.cpp"

EXECTUABLE="test_methods"

OUTPUT_FILE="results.txt"


echo "Kompilowanie programu..."

g++ -o "$EXECTUABLE" "$SOURCE_FILE" -std=c++20

if [ -f "$OUTPUT_FILE" ]; then
    rm "$OUTPUT_FILE"
fi

if [ "$#" -eq 0 ]; then
    echo "Brak podanych metod"
    exit 1
fi

for method in "$@"; do
    echo "Uruchamianie testu dla $method..."
    
    start_ms=$(ruby -e 'puts (Time.now.to_f * 1000).to_i')
    
    ./"$EXECTUABLE" "$method" > /dev/null 2>&1
    
    end_ms=$(ruby -e 'puts (Time.now.to_f * 1000).to_i')
    compare_time=$((end_ms - start_ms))
    
    memory_usage=$(ps -p $$ -o rss=)

    echo "Metoda: $method" >> "$OUTPUT_FILE"
    echo "Czas wykonania: $compare_time ms" >> "$OUTPUT_FILE"
    echo "Zużycie pamięci: $memory_usage KB" >> "$OUTPUT_FILE"
done