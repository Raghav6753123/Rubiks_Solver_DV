FROM python:3.11-slim

WORKDIR /app

# Install build essentials for C++
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Copy backend files
COPY requirements.txt .
COPY solver.cpp .
COPY main.py .

# Install Python dependencies
RUN pip install --no-cache-dir -r requirements.txt

# Compile solver
RUN g++ -O3 -std=c++17 -o solver solver.cpp

# Create pdb directory
RUN mkdir -p pdb

# Expose port
EXPOSE 8000

# Run the application
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]
