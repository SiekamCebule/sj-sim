# sj-sim
Symulator skoków narciarskich z interfejsem graficznym stworzonym w QT.
Ski jumping results simulator with GUI created in QT.

# Opcje w menu głównym
- **Symuluj sezon (jeszcze niedostępne)** - tryb, w którym tworzy się kalendarz, symulując po koleji konkursy dla różnych klasyfikacji mając po drodze wiele funkcji zwiększających realistykę (system automatycznego generowania formy skoczków, wykresy, statystyki, manipulowanie wynikami, zmiana bazy danych w trakcie sezonu)
- **Pojedynczy konkurs (jeszcze niedostępne)** - tryb, w którym rozgrywa się wyłącznie jeden konkurs, na określonej skoczni, z określoną listą startową.
- **Pojedyncze skoki** - użytkownik ma możliwość wybrania skoczni, jednego zawodnika, warunków wietrznych i przesymulowania określonej ilości skoków. Pokazywane są potem szczegółowe dane o każdym skoku, jak i wykresy czy ogólne statystyki.
- **Edytor bazy danych** - w oknie edytora bazy danych użytkownik edytuje globalną listę zawodników, skoczni, i zasad konkursów.
- **Pomoc (jeszcze niedostępne)** - wyjaśnione są tam trudniejsze zagadnienia dotyczące symulacji czy obsługi programu.

# Wyjaśnienie symulacji skoku
Symulacja skoku przebiega w stosunkowo prosty sposób. **Tak zwaną "ocenę wybicia", i "ocenę lotu" mnoży się przez "wpływ wybicia" i "wpływ lotu"** (atrybuty skoczni). Na końcu dodaje lub odejmuje się jeszcze odległość na podstawie wiatru.
## Ocena wybicia i ocena lotu
Są to parametry obliczane w głównej mierze ze swoich odpowiedników w postaci np. "Techniki lotu" czy "Siły wybicia".<br>
#### Ocena wybicia
**Ocena wybicia** = (1.0 * Technika wybicia) + (0.5 * Siła wybicia) + (0.7 * Forma), gdzie każdy poziom cechy charakterystycznej np. "Zwiększony wpływ techniki wybicia", przemnaża ją przez dodatkowe 0.1 (Dla poziomu 2, łącznie będzie to *Technika wybicia razy 1.2*<br>
Następnie ocena wybicia jest zmieniana w zależności od cechy "Wysokość wyjścia z progu" w zależności od wielkości skoczni (czym większa skocznia, tym lepszy niższy lot)<br>
#### Ocena lotu
**Ocena lotu** = (1 * Technika lotu) + (1.2 * Forma)
Tak samo jak z wybiciem, cecha charakterystyczna "wysokość lotu" ma swój wpływ na odległość.

*Obie te oceny są potem zmieniane przez dodatkową losowość skoku. Sama losowość jest też zależna od cechy charakterystycznej dot. wysokości wyjścia z progu i wysokości lotu. Im wyżej z progu wychodzi skoczek, lub wyżej leci, tym ma równiejsze skoki, ale nie zawsze jest w nich "błysk". Za to skoczek lecący niżej częściej może skok zepsuć ale też oddać lepszy "Bardzo dobry" skok.*


