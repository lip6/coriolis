# LSXLIB Spice Simulation

- Date : 2024-08-03
- Author : Franck Wajsburt with help of Marie-Minerve Louerat

## spi Directory

- All the `.spi` files originate from `coriolis-2.x/src/alliance-check-toolkit/cells/lsxlib/check`.
- This directory is created by running:
  ```sh
  make characterize
  ```
  in `coriolis-2.x/src/alliance-check-toolkit/cells/lsxlib`.
- The `characterize` command generates `lsxlib.lib`, and thus the `.spi` files are produced.

## cells Directories

- There is one directory per cell.
- In each directory, there is a `cell.cir` file which:
  - Instantiates the `../spi/cell.spi` model.
  - Defines the simulation conditions (technology, temperature, voltage, model: slow, typical, or fast).
  - Defines the stimuli.
  - Defines the measurement parameters.
  - Requests gnuplot to draw the stimuli.

## Makefile

- There is a single Makefile located in the root directory.
- For each cell:
  - It navigates to the directory dedicated to the cell.
  - It performs three simulations: slow, typical, and fast.
- Usage:
  - `make` : Simulates all cells.
  - `make C=inv_x4` : Simulates only one cell (here `inv_x4`)
  - `make clean` : Cleans all files, except the source files.
