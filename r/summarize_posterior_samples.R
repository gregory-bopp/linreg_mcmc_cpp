##
## Plot posterior samples against ground truth
##

library(tidyverse)
library(here)
proj_dir <- here()
data_dir <- file.path(proj_dir, 'data')
sep <- ','


# Load posterior samples --------------------------------------------------
post_beta <- read.table(file.path(data_dir, 'posterior_beta.csv'),sep = sep)
post_sigma_sq <- read.table(file.path(data_dir, 'posterior_sigma_sq.csv'), sep = sep)%>% unlist


# Load ground truth -------------------------------------------------------
gt <- readRDS(file = file.path(data_dir, 'ground_truth.rds'))

# Load data ---------------------------------------------------------------
y <- read.table(file.path(data_dir, 'y.csv'), sep = sep) %>% as.matrix()
X <- read.table(file.path(data_dir, 'X.csv'), sep = sep) %>% as.matrix()


# Plot posterior samples against ground truth -----------------------------
post_tib <- post_beta %>%
  as_tibble %>%
  setNames(paste0('beta', 1:ncol(post_beta))) %>%
  add_column(sample_index = 1:nrow(post_beta))%>%
  pivot_longer(cols = starts_with('beta'),
               names_to = "param",
               values_to = "post_sample") 
ggplot(post_tib) +
  geom_line(aes(x = sample_index, 
                y = post_sample, 
                group = param, 
                color = param)) +
  geom_hline(yintercept = gt$beta) +
  theme_bw() +
  labs(y = "Posterior Sample", 
       x = "Sample Index",
       color = "Parameter")

