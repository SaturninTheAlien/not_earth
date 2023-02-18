# Nie-ziemia, instalacja artystyczna
Program, który w założeniu miał być symulacją Układu Słonecznego.
Jednak z uwagi na proporcje astronomicznych wielkości takich jak np.
promień Ziemi i odleglość promień orbity, czy prędkość kątowa Ziemi po orbicie
i w ruchu obrotowym, wyglądało to niezbyt ładnie.
Zatem zamiast modelu Układu Słonecznego zdecydowałem się zrobić sztukę nowoczesną
(niektóre wielkości ustawiłem czysto fikcyjne, nie implementowałem na razie pozostałych "nie-planet").

## Wymagane pakiety apt-get:

- libglfw3-dev
- libglew-dev
- libassimp-dev
- pybind11-dev
- python3-dev
- python3-numpy
- python3-pillow
- python3-pil

## Kompilacja i uruchomienie
> make
> ./not_earth.x

## Sterowanie
Można obracać myszką po wciśnięciu kilknięciuś kółka (jak w Blender3D).

Klawiatura:
1. "z" - obraca widok tak, że kieruje oś "z" w kierunku obserwatora, podobnie "x" i "y",
1. "r" - ukrywa/pokazuje różę wiatrów,
1. "m" - zastępuje księżyc ksienrzycem (lub odwrotnie),
1. "w" - przyspiesza symulacja 2 razy,
1. "s" - zwalnia symulację 2 razy,