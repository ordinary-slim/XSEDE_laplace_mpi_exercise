## Description

Parallel implementation (MPI) of a finite difference solution of
Laplace problem over the unit square. Boundary conditions are:

- Left & top: $T = 0$
- Bottom: $T = 100 / x$
- Right: $T = 100 / y$
- Null initial condition for interior points

Exercise is proposed by [this YouTube video](https://www.youtube.com/watch?v=SSloT4nd9fU).

#### Solution using 20 CPUS

Partition           |  Temperature field
:-------------------------:|:-------------------------:
<img src="https://user-images.githubusercontent.com/55497778/192350116-51c2b8ba-cf3f-485c-abeb-54b1c9a096a9.png" style="background-color: white;">  |  <img src="https://user-images.githubusercontent.com/55497778/192350118-1af0bba2-0a9f-4801-a890-9c66e041e1b8.png" style="background-color: white;">

#### Strong scaling
Up to 128 processors
<img src="https://user-images.githubusercontent.com/55497778/194728205-b44dc6c3-1d25-490f-b9cb-9841e7b64747.png" style="background-color: white;">

## Developper notes

### Checkpoints

- [x] Parallel version runs correctly
- [x] Parallel version runs
- [x] Serial version runs correctly

### TODO

- [x] Add strong scaling plot
- [ ] // vtk
- [x] Ghost cell information communication
- [x] Parallelization game-plan
- [ ] Write lighter vtk for visualization (_low priority_)
- [x] Initial condition set correctly in //
- [x] Fix bug final error seems too big (_high priority_)
- [x] Write vtk for visualization (_high priority_)
