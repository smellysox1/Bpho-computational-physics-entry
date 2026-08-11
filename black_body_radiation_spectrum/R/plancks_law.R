if (!exists('T1')) {
  T1 = 6000
}
if (!exists('T2')) {
  T2 = 5000
}
if (!exists('T3')) {
  T3 = 4000
}

par(mar=c(5, 5, 1, 1))

k_B <- 1.381e-23          # J K^-1
h <- 6.626e-34            # J s
lightspeed <- 2.998e+8    # m s^-1

plotPlancksLaw <- function(temp, colour, add) {
  curve(
    (2 * h * (lightspeed ^ 2) / ((x * 10^-9) ^ 5)) / (exp(h * lightspeed / (x * 10^-9 * k_B * temp)) - 1),
    from=0, to=2500,
    xlab='Wavelength, λ / nm', ylab=expression(paste('Irradiance, I / W m'^-2)),
    add=add, col=colour
  )
}

plotPlancksLaw(T1, 'green', FALSE)
plotPlancksLaw(T2, 'blue', TRUE)
plotPlancksLaw(T3, 'red', TRUE)

legend('topright', title=NULL, legend=c(paste('T=', T3, 'K'), paste('T=', T2, 'K'), paste('T=', T1, 'K')), col=c('red', 'blue', 'green'), lty=1:1)