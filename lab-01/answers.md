### File: `w01-byte.c`

| S. No.   | Data amount | Time taken | Speed          |
| -------- | ----------- | ---------- | -------------- |
| Exp 1    | 4096 bytes  | 161.492 s  | 25.3635 byte/s |
| Exp 2    | 8192 bytes  | 322.440 s  | 25.4063 byte/s |
| Exp 3    | 12288 bytes | 484.994 s  | 25.3364 byte/s |

### File: `w02-byte.c`

| S. No.   | Data amount    | Time taken | Speed         |
| -------- | -------------- | ---------- | ------------- |
| Exp 1    | 5120000 bytes  | 5.670 s    | 902971 byte/s |
| Exp 2    | 5120000 bytes  | 5.522 s    | 927197 byte/s |
| Exp 3    | 5120000 bytes  | 5.615 s    | 911895 byte/s |

### File: `w03-byte.c`

| S. No.   | Data amount    | Time taken | Speed              |
| -------- | -------------- | ---------- | ------------------ |
| Exp 1    | 5120000 bytes  | 0.014 s    | 3.72756e+08 byte/s |
| Exp 2    | 5120000 bytes  | 0.017 s    | 2.98668e+08 byte/s |
| Exp 3    | 5120000 bytes  | 0.023 s    | 2.22988e+08 byte/s |

## Matrix Multiplication
| S. No.   | Configuration  | n    | Speed         |
| -------- | -------------- | ---- | ------------- |
| Exp 1    | ijk            | 1000 | 0.589 Gflop/s |
|          | ikj            | 1000 | 0.308 Gflop/s |
|          | jik            | 1000 | 0.536 Gflop/s |
|          | jki            | 1000 | 0.718 Gflop/s |
|          | kij            | 1000 | 0.307 Gflop/s |
|          | kji            | 1000 | 0.730 Gflop/s |
|          |                |      |               |
| Exp 2    | ijk            | 1000 | 0.594 Gflop/s |
|          | ikj            | 1000 | 0.302 Gflop/s |
|          | jik            | 1000 | 0.572 Gflop/s |
|          | jki            | 1000 | 0.728 Gflop/s |
|          | kij            | 1000 | 0.297 Gflop/s |
|          | kji            | 1000 | 0.741 Gflop/s |
|          |                |      |               |
| Exp 3    | ijk            | 1000 | 0.595 Gflop/s |
|          | ikj            | 1000 | 0.299 Gflop/s |
|          | jik            | 1000 | 0.576 Gflop/s |
|          | jki            | 1000 | 0.748 Gflop/s |
|          | kij            | 1000 | 0.325 Gflop/s |
|          | kji            | 1000 | 0.749 Gflop/s |
|          |                |      |               |
| Exp 4    | ijk            | 1000 | 0.573 Gflop/s |
|          | ikj            | 1000 | 0.298 Gflop/s |
|          | jik            | 1000 | 0.557 Gflop/s |
|          | jki            | 1000 | 0.733 Gflop/s |
|          | kij            | 1000 | 0.303 Gflop/s |
|          | kji            | 1000 | 0.709 Gflop/s |
|          |                |      |               |
| Exp 5    | ijk            | 1000 | 0.586 Gflop/s |
|          | ikj            | 1000 | 0.301 Gflop/s |
|          | jik            | 1000 | 0.560 Gflop/s |
|          | jki            | 1000 | 0.744 Gflop/s |
|          | kij            | 1000 | 0.305 Gflop/s |
|          | kji            | 1000 | 0.742 Gflop/s |

As we can see from the experiments, the order is:
```
ikj ~ kij < jik < ijk < jki ~ kji
```
