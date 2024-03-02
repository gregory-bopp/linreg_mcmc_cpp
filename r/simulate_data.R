##
## Simulate data from linear model:
##      y ~ X%*%beta + eps
##      eps ~ N(0,sigma_sq)
##

library(here)
# Path to top-level project directory
proj_dir <- here()                      
data_dir <- file.path(proj_dir, 'data')

# Save data as csvs
sep <- ','

# Set random seed for reproducibility
set.seed(1)

# Ground Truth -----------------------------------------------------------
n <- 500
sigma_sq <- 5
beta <- c(0.5, 1, -2, 3, 4)                               # Coefficients
X <- cbind(1, matrix(rnorm((length(beta)-1) * n), nrow = n))  # Covariates
y <- X %*% beta + rnorm(n, sd = sqrt(sigma_sq))           # Response
gt <- list(beta = beta,
            sigma_sq = sigma_sq,
            X = X,
            y = y)
saveRDS(object = gt, file = file.path(data_dir, 'ground_truth.rds'))

# Write to file -----------------------------------------------------------
write.table(
  X,
  file = file.path(data_dir, 'X.csv'),
  sep = sep,
  row.names = F,
  col.names = F
)
write.table(
  y,
  file = file.path(data_dir, 'y.csv'),
  sep = sep,
  row.names = F,
  col.names = F
)


# Maximum Likelihood/Least Squares Solution----------------------------------
lm(y~-1 + X)

