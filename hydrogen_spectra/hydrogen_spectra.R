
# Constants

R <- 1.09677e7          # Rydberg constant                        / m^-1
h <- 6.62607015e-34     # Planck constant                         / J s
c <- 299792458          # Speed of light in vacuo                 / m s^-1
e <- 1.602176634e-19    # Magnitude of the charge of the electron / C



# Rydberg formula
# m represents the principal quantum number of the lower energy level
# n represents the principal quantum number of the upper energy level
rydberg <- function(m, n) {
  return (1e9 / (R * (1 / m^2 - 1 / n^2)))
}

# Photon energy, using the Rydberg formula
# m represents the principal quantum number of the lower energy level
# n represents the principal quantum number of the upper energy level
energy <- function(m, n) {
  return (h * c * R * (1 / m^2 - 1 / n^2) / e)
}

# Lyman series
# m = 1, n >= 2
# Ultraviolet light
lyman_x <- c(
  rydberg(1, 2),
  rydberg(1, 3),
  rydberg(1, 4),
  rydberg(1, 5)
)

lyman_y <- c(
  energy(1, 2),
  energy(1, 3),
  energy(1, 4),
  energy(1, 5)
)

# Balmer series
# m = 2, n >= 3
# First four are visible light (ranging from red to violet), and after that is ultraviolet
balmer_x <- c(
  rydberg(2, 3),
  rydberg(2, 4),
  rydberg(2, 5),
  rydberg(2, 6)
)

balmer_y <- c(
  energy(2, 3),
  energy(2, 4),
  energy(2, 5),
  energy(2, 6)
)

# Paschen series
# m = 3, n >= 4
# Infrared light
paschen_x <- c(
  rydberg(3, 4),
  rydberg(3, 5),
  rydberg(3, 6),
  rydberg(3, 7)
)

paschen_y <- c(
  energy(3, 4),
  energy(3, 5),
  energy(3, 6),
  energy(3, 7)
)

# Brackett series
# m = 4, n >= 5
# Infrared light
brackett_x <- c(
  rydberg(4, 5),
  rydberg(4, 6),
  rydberg(4, 7),
  rydberg(4, 8)
)

brackett_y <- c(
  energy(4, 5),
  energy(4, 6),
  energy(4, 7),
  energy(4, 8)
)

# Pfund series
# m = 5, n >= 6
# Infrared light
pfund_x <- c(
  rydberg(5, 6),
  rydberg(5, 7),
  rydberg(5, 8),
  rydberg(5, 9)
)

pfund_y <- c(
  energy(5, 6),
  energy(5, 7),
  energy(5, 8),
  energy(5, 9)
)

plot(
  NULL,
  xlim=range(c(lyman_x, balmer_x, paschen_x, brackett_x, pfund_x)), 
  ylim=range(c(lyman_y, balmer_y, paschen_y, brackett_y, pfund_y)),
  xlab="Wavelength, λ / nm", ylab="Photon energy, E / eV"
)

points(lyman_x,    lyman_y,    pch=4, col="magenta")
points(balmer_x,   balmer_y,   pch=4, col="red")
points(paschen_x,  paschen_y,  pch=4, col="blue")
points(brackett_x, brackett_y, pch=4, col="green")
points(pfund_x,    pfund_y,    pch=4, col="orange")

abline(v=lyman_x,    col="magenta", lty=2)
abline(v=balmer_x,   col="red",     lty=2)
abline(v=paschen_x,  col="blue",    lty=2)
abline(v=brackett_x, col="green",   lty=2)
abline(v=pfund_x,    col="orange",  lty=2)

legend("topright", legend=c("Lyman", "Balmer", "Paschen", "Brackett", "Pfund"), pch=4, 
       col=c("magenta", "red", "blue", "green", "orange"))
