if (!exists('n')) {
  n = 1
}

h <- 6.626e-34           # Planck constant                         / J s
m_e <- 9.1094e-31        # Mass of the electron                    / kg

d <- 0.123e-9            # Atomic spacing

curve(
  x / (n * h / (2 * d * sqrt(2 * m_e))),
  xlab='sin(phi/2)', ylab='1 / sqrt(V)',
  from=0, to=1
)