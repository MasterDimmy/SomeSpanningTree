#ifndef SETS_H
#define SETS_H

/*
	here we working with sets
*/

typedef struct 
{
    int size; 
    int* s;
} Sets;

/* initialize set */
void set_init(Sets* st, int n)
{	int i;
	st->size = n;
	st->s = (int*) malloc(n*sizeof(int));
	if (st->s == 0) {
		perror("malloc");
		exit(EXIT_FAILURE); 
	}
	/* no any data: -1 value */
	for (i=0; i<n; i++)
          st->s[i] = -1;
}

/* search element's end in set */
int set_find( Sets* st, int x )
{
       int temp = x;
       while (st->s[temp] >= 0)
            temp = st->s[temp];       
       return temp;
}

/* union to sets - make junction from one to other */
void set_union( Sets* st, int root1, int root2)
{
    if (st->s[root2] < st->s[root1])
        st->s[root1] = root2;      
    else {
        if (st->s[root1] == st->s[root2] )
            st->s[root1]--;          
        st->s[root2] = root1;      
    }
}

/* clear set */
void set_clear(Sets *s) {
	if (s->size!=0)
		free(s->s);
}

#endif
