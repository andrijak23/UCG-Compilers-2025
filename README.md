# Kompajler Projekat 2025

Ovaj projekat predstavlja **mini programski jezik za upite** i njegovu potpunu implementaciju uključujući leksičku analizu, parsiranje i gradnju apstraktnog sintaksnog stabla (AST). Napisan je pomoću **Flexa (scanner.l)**, **Bison-a (parser.y)** i C jezika.

---

## Struktura projekta

```
.
├── ast.c / ast.h               # AST struktura i funkcije za izgradnju i štampu stabla
├── errors.c / errors.h         # Prikaz grešaka tokom analize
├── parser.y                    # Bison gramatika (sintaksni analizator)
├── scanner.l                   # Flex leksički analizator
├── location.h                  # Lokacija tokena (linija, kolona)
├── main.c                      # Ulazna tačka (poziva yyparse)
├── build.bat                   # Skripta za Windows kompajliranje i pokretanje
└── test.txt                    # Ulazni test fajl
```

---

## Šta ovaj jezik podržava?

- Definisanje upita:  
  ```txt
  QUERY q1 = <apple banana | orange>;
  ```

- Liste upita:  
  ```txt
  QUERY qlist = [q1, q2];
  ```

- Operacije nad rezultatima:  
  ```txt
  z = x ++ y -- w ** v;
  ```

- Uslovni i iterativni blokovi:
  ```txt
  IF EMPTY(x) BEGIN
      EXEC q1;
  END

  FOR item IN [q1, q2] BEGIN
      EXEC item;
  END
  ```

---

## Kako pokrenuti?

1. Otvori **Command Prompt (CMD)** u folderu projekta.
2. Pokreni skriptu:
   ```bash
   build.bat
   ```
3. Ako želiš da automatski testiraš:
   - Odgovori `y` kada te skripta pita da li želiš da pokreneš program.

---

## Primjer ulaza (`test.txt`)

```txt
QUERY q1 = <+alpha beta | gamma>;
QUERY q2 = <filetype:pdf apple>;

FOR item IN [q1, q2] BEGIN
    x = EXEC item;
    result = x ++ item;
END
```

---

## Šta radi parser?

- Lex vrši tokenizaciju ulaza.
- Bison parsira u skladu s pravilima gramatike.
- AST se generiše i ispisuje u obliku stabla:
  ```
  Program
    Declarations
      QueryDeclaration: q1
      ...
    Commands
      ForCommand: item
      ...
  ```

---

## Napomena

Projekat je napisan za završni rad iz predmeta [Programski Prevodioci](https://www.ucg.ac.me/predmet/7/1/3/2017/1348-compilers).

