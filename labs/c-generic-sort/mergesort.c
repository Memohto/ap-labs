
void merge(void *lineptr[], int l, int m, int r, int (*comp)(void *, void *))
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
  
    void *L[n1];
		void *R[n2];
  
    for (i = 0; i < n1; i++) {
			L[i] = lineptr[l + i];
		}
    for (j = 0; j < n2; j++) {
			R[j] = lineptr[m + 1 + j];
		}
  
    i = 0;
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) {
        if ((*comp)(L[i], R[j]) <= 0) {
            lineptr[k] = L[i];
            i++;
        }
        else {
            lineptr[k] = R[j];
            j++;
        }
        k++;
    }
  
    while (i < n1) {
        lineptr[k] = L[i];
        i++;
        k++;
    }
  
    while (j < n2) {
        lineptr[k] = R[j];
        j++;
        k++;
    }
}
  
void mergesort(void *lineptr[], int left, int right, int (*comp)(void *, void *)) {
    if (left < right) {
        int middle = left + (right - left) / 2;
  
        mergesort(lineptr, left, middle, comp);
        mergesort(lineptr, middle + 1, right, comp);
  
        merge(lineptr, left, middle, right, comp);
    }
}