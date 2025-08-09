# SinDDS-DSK6713
DDS of Sinusoidal wave

It is a type of deterministic signal that is formed from the sine function considering a frequency, a phase at time t, and is proportional to an amplitude value. It is defined as:

$x(t)=A*sin(ωt+θ)$

Where the angular frequency $ω=2πf$ and $θ$ is the phase, that is, the displacement of the signal in time from an initial reference.

## Design and implementation
This implementation uses a lookup table to predefine the values of the equispaced sine wave signal defined within the sine wave circle. In this way, multiples of the original sampling frequency of the codec can be obtained, as well as the definition of the frequency value based on the following formula:

$f_{out}=\frac{f_s}{sinTableLen*frequency}$

## Results

It can be seen that the signal is generated correctly. Upon closer analysis, the effect of the steps between transitions from one sample to another is noticeable, but the signal obtained is functional and correct.

<img width="961" height="520" alt="image" src="https://github.com/user-attachments/assets/2515813a-04e2-407f-a123-806a7a5efbfa" />
