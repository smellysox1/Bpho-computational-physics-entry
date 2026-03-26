temp <- 6000              # K

k_B <- 1.381e-23          # J K^-1
h <- 6.626e-34            # J s
lightspeed <- 2.998e+8    # m s^-1

curve(
  (2 * h * (lightspeed ^ 2) / ((x * 10^-9) ^ 5)) / (exp(h * lightspeed / (x * 10^-9 * k_B * temp)) - 1),
  from=0, to=2500,
  xlab="Wavelength, λ / nm", ylab="Irradiance, I / W m^-2"
)

temp <- 5000              # K

curve(
  (2 * h * (lightspeed ^ 2) / ((x * 10^-9) ^ 5)) / (exp(h * lightspeed / (x * 10^-9 * k_B * temp)) - 1),
  from=0, to=2500,
  add=TRUE
)

temp <- 4000              # K

curve(
  (2 * h * (lightspeed ^ 2) / ((x * 10^-9) ^ 5)) / (exp(h * lightspeed / (x * 10^-9 * k_B * temp)) - 1),
  from=0, to=2500,
  add=TRUE
)