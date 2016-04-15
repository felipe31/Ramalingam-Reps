library("fitdistrplus")

args <- commandArgs(TRUE)

numero_vertices <- as.numeric(args[1])

grau<-rpois(numero_vertices, 4.809404)#grau dos vertices

#etx<-rlnorm(numero_vertices*(numero_vertices-1), 0.5921132, 1.0060179)#valores de etx

etx<-rlnorm(sum(grau), 0.5921132, 1.0060179)#valores de etx

#etx<-runif(n=sum(grau), min=1, max=numero_vertices)

write(grau, "distribuicao_grau_vertices.txt", ncolumns=1)

write(etx, "distribuicao_etx.txt", ncolumns=1)
