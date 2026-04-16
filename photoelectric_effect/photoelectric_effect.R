library(hash)

electronCharge <- 1.6e-19   # C
h <- 6.626e-34                         # J s

ag <- hash()

ag[["name"]] <- "Silver"
ag[["colour"]] <- "red"
ag[["workFunction"]] <- 4.3

al <- hash()

al[["name"]] <- "Aluminium"
al[["colour"]] <- "blue"
al[["workFunction"]] <- 4.3

au <- hash()

au[["name"]] <- "Gold"
au[["colour"]] <- "green"
au[["workFunction"]] <- 5.1

cu <- hash()

cu[["name"]] <- "Copper"
cu[["colour"]] <- "cyan"
cu[["workFunction"]] <- 4.7

sn <- hash()

sn[["name"]] <- "Tin"
sn[["colour"]] <- "purple"
sn[["workFunction"]] <- 4.4

pb <- hash()

pb[["name"]] <- "Lead"
pb[["colour"]] <- "turquoise"
pb[["workFunction"]] <- 4.3

w <- hash()

w[["name"]] <- "Tungsten"
w[["colour"]] <- "orange"
w[["workFunction"]] <- 4.5

ni <- hash()

ni[["name"]] <- "Nickel"
ni[["colour"]] <- "magenta"
ni[["workFunction"]] <- 4.6

na <- hash()

na[["name"]] <- "Sodium"
na[["colour"]] <- "black"
na[["workFunction"]] <- 2.4
  
materials <- c(ag, al, au, cu, sn, pb, w, ni, na)

plotPhotoelectricEffect <- function(W, col, add) {
  curve(
    h * (x * 10^15) / electronCharge - W,
    from=0, to=2.5,
    col=col, add=add,
    xlab="Frequency, f / Hz",
    ylab="Stopping voltage, V / V"
  )
}

graphExists <- FALSE
for (mat in materials) {
  plotPhotoelectricEffect(mat[["workFunction"]], mat[["colour"]], graphExists)
  graphExists <- TRUE
}

matNames <- c()
colours <- c()

for (mat in materials) {
  matNames <- c(matNames, mat[["name"]])
  colours <- c(colours, mat[["colour"]])
}

legend("topright", legend=matNames, col=colours, lty=1:1)