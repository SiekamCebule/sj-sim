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
**Ocena wybicia** = (1.0 * Technika wybicia) + (0.5 * Siła wybicia) + (0.7 * Forma), gdzie każdy poziom cechy charakterystycznej np. "Zwiększony wpływ techniki wybicia", przemnaża ją przez dodatkowe 0.1 (Dla poziomu 2, łącznie będzie to *Technika wybicia razy 1.2*<br>
Następnie ocena wybicia jest zmieniana w zależności od cechy "Wysokość wyjścia z progu" w zależności od wielkości skoczni (czym większa skocznia, tym lepszy niższy lot)<br>
**Ocena lotu** = (1 * Technika lotu) + (1.2 * Forma)
Tak samo jak z wybiciem, cecha charakterystyczna "wysokość lotu" ma swój wpływ na odległość.

*Obie te oceny są potem zmieniane przez dodatkową losowość skoku. Sama losowość jest też zależna od cechy charakterystycznej dot. wysokości wyjścia z progu i wysokości lotu. Im wyżej z progu wychodzi skoczek, lub wyżej leci, tym ma równiejsze skoki, ale nie zawsze jest w nich "błysk". Za to skoczek lecący niżej częściej może skok zepsuć ale też oddać lepszy "Bardzo dobry" skok.*

# Umiejętności zawodnika
- Siła wybicia
- Technika wybicia
- Technika lotu
- Styl lotu
- Styl lądowania (**Wyjątkowo, ZAKRES OD 1 DO 15**) (teorytycznie do 20 ale są to wtedy "niespotykani" obecnie styliści)
- Forma

## Styl lotu skoczka
Wyróżniami 4 style lotu:
- Styl V (Np. Japończycy)
- Nowoczesne V (Np. Kamil Stoch)
- Szerokie V (Np. Domen Prevc)
- Styl H (Np. Danil Vassilyev)
*Im większa skocznia, tym większe bonusy mają "szersze" style, tak samo im lepsze warunki tym większy wpływ mają style bardziej nowoczesne.*<br>
**Nie ma żadnej górnej granicy umiejętności, ale osobiście zalecam zakres od 1 do 60, z przekroczeniem 60 tylko w przypadku wybitnych zawodników.**

# Parametry skoczni (symulacja, czyli pomijając przeliczniki za belkę itd.)
- Wpływ wybicia (przemnażany przez ocenę wybicia)
- Wpływ lotu (przemnażany przez ocenę lotu)
## Jak dopasować wpływ wybicia i lotu do symulacji?
Osobiście, ustalam wartość koło 0.6 - 0.65 dla skoczni dużej z punktem K koło 120-125 metra, i proporcjonalnie do zmiany punktu K zmieniam tą wartość (Np. dla skoczni mamuciej K200 koło 1.00 - 1.05).
Później tą wartość, np. 0.62 rozdzielam tak, aby wybicie i lot miało taki wpływ jak w rzeczywistości, np 40% wybicie i 60% lot czyli coś koło 0.28 dla wybicia i 0.34 dla lotu.) I np. dla bardziej lotnych skoczni jak Sapporo zwiększamy wpływ lotu odejmując jednocześnie wpływ wybicia. (Można się też pobawić cechami charakterystycznymi skoczni, np. dla lotnej skoczni zwiększamy cechę "Wpływ techniki lotu na skok" do poziomu np. 1.5).<br>
