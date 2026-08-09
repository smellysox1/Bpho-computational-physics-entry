R <- 8.314                # J mol^-1 K^-1

plotEinsteinMolarHeatCapacity <- function(T_E, colour, add) {
  curve(
    3 * R * (T_E / x) ^ 2 * exp(T_E / x) / (exp(T_E / x) - 1) ^ 2,
    from=0, to=800,
    xlab='Temperature, T / K', ylab=expression(paste('Molar heat capacity, C / J mol'^-1, 'K'^-1)),
    add=add, col=colour
  )
}

plotEinsteinMolarHeatCapacity(170 * (pi / 6) ^ (1/3), 'red', FALSE) # gold
plotEinsteinMolarHeatCapacity(343.5 * (pi / 6) ^ (1/3), 'blue', TRUE) # copper
plotEinsteinMolarHeatCapacity(420 * (pi / 6) ^ (1/3), 'green', TRUE) # titanium
plotEinsteinMolarHeatCapacity(428 * (pi / 6) ^ (1/3), 'yellow', TRUE) # aluminium
plotEinsteinMolarHeatCapacity(470 * (pi / 6) ^ (1/3), 'orange', TRUE) # iron
plotEinsteinMolarHeatCapacity(645 * (pi / 6) ^ (1/3), 'purple', TRUE) # silicon
plotEinsteinMolarHeatCapacity(2230 * (pi / 6) ^ (1/3), 'black', TRUE) # carbon

legend('topright', title=NULL, legend=c('Gold', 'Copper', 'Titanium', 'Aluminium', 'Iron', 'Silicon', 'Carbon'), col=c('red', 'blue', 'green', 'yellow', 'orange', 'purple', 'black'), lty=1:1)
