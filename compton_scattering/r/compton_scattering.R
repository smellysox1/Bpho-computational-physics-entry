
m_e <- 9.1094e-31        # Mass of the electron                    / kg
c <- 2.998e+8            # Speed of light                          / m s^-1
e <- 1.602176634e-19     # Magnitude of the charge of the electron / C
h <- 6.626e-34           # Planck constant                         / J s

E_e <- m_e * c * c       # Rest energy of the electron             / J
hc <- h * c              # h × c                                   / J m

showLegend <- function() {
  legend('topright', title=NULL, legend=c('E=50keV', 'E=100keV', 'E=200keV', 'E=500keV', 'E=1000keV'), col=c('green', 'blue', 'red', 'purple', 'orange'), lty=1:1)
}


Δλ <- function(θ) {
  return (h / (m_e * c) * (1 - cos(θ)))
}

plotFractionalWavelengthShift <- function(energy, colour, add) {
  E_λ <- energy * e * 1000

  curve(
    E_λ * Δλ(x) / hc,
    from=0, to=pi,
    xlab="Photon scattering angle θ/rad", ylab="Δλ/λ",
    col=colour,
    add=add
  )
}

plotElectronRecoilSpeed <- function(energy, colour, add) {
  E_λ <- energy * e * 1000
  λ <- hc / E_λ

  curve(
    sqrt(1 - (E_e / (E_λ - hc / (Δλ(x) + λ) + E_e))^2),
    from=0, to=pi,
    xlab="Photon scattering angle θ/rad", ylab="Electron recoil speed v/c",
    col=colour,
    add=add
  )
}

plotRecoilAngle <- function(energy, colour, add) {
  E_λ <- energy * e * 1000
  λ <- hc / E_λ
  
  curve(
    atan(sin(x) / (1 + Δλ(x) / λ - cos(x))),
    from=0, to=pi,
    xlab="Photon scattering angle θ/rad", ylab="Electron recoil angle φ/rad",
    col=colour,
    add=add
  )
}

plotFractionalWavelengthShift(1000, "orange", FALSE)
plotFractionalWavelengthShift(500, "purple", TRUE)
plotFractionalWavelengthShift(200, "red", TRUE)
plotFractionalWavelengthShift(100, "blue", TRUE)
plotFractionalWavelengthShift(50, "green", TRUE)

showLegend()

plotElectronRecoilSpeed(1000, "orange", FALSE)
plotElectronRecoilSpeed(500, "purple", TRUE)
plotElectronRecoilSpeed(200, "red", TRUE)
plotElectronRecoilSpeed(100, "blue", TRUE)
plotElectronRecoilSpeed(50, "green", TRUE)

showLegend()

plotRecoilAngle(1000, "orange", FALSE)
plotRecoilAngle(500, "purple", TRUE)
plotRecoilAngle(200, "red", TRUE)
plotRecoilAngle(100, "blue", TRUE)
plotRecoilAngle(50, "green", TRUE)

showLegend()
