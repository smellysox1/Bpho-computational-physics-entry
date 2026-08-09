
# Constants

pi    <- 3.1415926535897932385

h_bar <- 1.054571817e-34           # Reduced Planck constant   / J s
m     <- 9.1094e-31                # Mass of the electron      / kg
a     <- 0.55e-10                  # Width of the box          / m
e     <- 1.602176634e-19           # Charge of the electron    / C


# define margins so that exponents in axis labels aren't cut off
par(mar=c(5, 5, 1, 1))

# n represents the quantum number
energy <- function(n) {
  return (h_bar^2 * pi^2 * n^2 / (2 * m * a^2 * e))
}

curve(
  energy(x),
  from=0, to=3,
  xlab="Quantum number, n", ylab="Energy, E / eV"
)

t <- 1
n <- 1

curve(
  (sqrt(2 / a) * cos(-energy(n) * t / h_bar) * sin(n * pi * x * 1e-10 / a))^2+
  (sqrt(2 / a) * sin(-energy(n) * t / h_bar) * sin(n * pi * x * 1e-10 / a))^2,
  from=0, to=a/1e-10,
  xlab="Displacement, x / Ångströms", ylab=expression(paste("Probability density, |ψ|"^2)),
  col="blue"
)

n <- 2

curve(
  (sqrt(2 / a) * cos(-energy(n) * t / h_bar) * sin(n * pi * x * 1e-10 / a))^2+
  (sqrt(2 / a) * sin(-energy(n) * t / h_bar) * sin(n * pi * x * 1e-10 / a))^2,
  col="green", add=TRUE
)

n <- 3

curve(
  (sqrt(2 / a) * cos(-energy(n) * t / h_bar) * sin(n * pi * x * 1e-10 / a))^2+
  (sqrt(2 / a) * sin(-energy(n) * t / h_bar) * sin(n * pi * x * 1e-10 / a))^2,
  col="red", add=TRUE
)