# Machine Learning Price Estimator

This C project implements a basic machine learning algorithm (linear regression with one-shot learning) to predict housing prices based on attributes like bedrooms, bathrooms, square footage, and year built.

### 🧮 How It Works
- Trains on a dataset of houses using:
  - Matrix multiplication
  - Transposition
  - Gauss-Jordan matrix inversion (implemented from scratch)
- Outputs price predictions for unseen houses

### 🔧 Features
- Works with `.txt` input files
- Uses double-precision arithmetic
- Rounded output to nearest integer
- No external libraries required

### 📄 Sample Training File Format
train 4 3 3.0 2.0 1500.0 1999.0 300000.0 2.0 1.0 900.0 1980.0 180000.0 4.0 3.0 2200.0 2005.0 500000.0

### 📄 Sample Input File
data 4 2 3.0 2.0 1400.0 2001.0 2.0 1.0 1000.0 1975.0


### 💡 Implementation Notes
- Manual memory allocation
- Input parsing via `fscanf`
- Prints predictions with `%.0f` for clean rounded output

> For learning purposes only. Implements one-shot linear regression with no external dependencies.

