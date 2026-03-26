
par(mar=c(5, 5, 1, 1))

k_B <- 1.381e-23          # J K^-1
h <- 6.626e-34            # J s
lightspeed <- 2.998e+8    # m s^-1

plotPlancksLaw <- function(temp, colour, add) {
  curve(
    (2 * h * (lightspeed ^ 2) / ((x * 10^-9) ^ 5)) / (exp(h * lightspeed / (x * 10^-9 * k_B * temp)) - 1), 
    from=0, to=2500,
    xlab="Wavelength, λ / nm", ylab=expression(paste("Irradiance, I / W m"^-2)),
    add=add, col=colour
  )
}

plotPlancksLaw(6000, "green", FALSE)
plotPlancksLaw(5000, "blue", TRUE)
plotPlancksLaw(4000, "red", TRUE)

legend("topright", title=NULL, legend=c("T=4000K", "T=5000K", "T=6000K"), col=c("red", "blue", "green"), lty=1:1)

R <- 8.314                # J mol^-1 K^-1

plotEinsteinMolarHeatCapacity <- function(T_E, colour, add) {
  curve(
    3 * R * (T_E / x) ^ 2 * exp(T_E / x) / (exp(T_E / x) - 1) ^ 2,
    from=0, to=800,
    xlab="Temperature, T / K", ylab=expression(paste("Molar heat capacity, C / J mol"^-1, "K"^-1)),
    add=add, col=colour
  )
}

plotEinsteinMolarHeatCapacity(170 * (pi / 6) ^ (1/3), "red", FALSE) # gold
plotEinsteinMolarHeatCapacity(343.5 * (pi / 6) ^ (1/3), "blue", TRUE) # copper
plotEinsteinMolarHeatCapacity(420 * (pi / 6) ^ (1/3), "green", TRUE) # titanium
plotEinsteinMolarHeatCapacity(428 * (pi / 6) ^ (1/3), "yellow", TRUE) # aluminium
plotEinsteinMolarHeatCapacity(470 * (pi / 6) ^ (1/3), "orange", TRUE) # iron
plotEinsteinMolarHeatCapacity(645 * (pi / 6) ^ (1/3), "purple", TRUE) # silicon
plotEinsteinMolarHeatCapacity(2230 * (pi / 6) ^ (1/3), "black", TRUE) # carbon

legend("topright", title=NULL, legend=c("Gold", "Copper", "Titanium", "Aluminium", "Iron", "Silicon", "Carbon"), col=c("red", "blue", "green", "yellow", "orange", "purple", "black"), lty=1:1)
