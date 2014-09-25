# Triadic analyses of affiliation networks



The paper "Triadic analysis for affiliation networks" will make a case for adopting a coherent batch of triad-centric tools for the study of two-mode, usually affiliation, networks. This R Markdown file applies a few of tools to the study of some manageably-sized real-world affiliation networks, in hopes of giving the reader a feel for what they mean, how they can be used, and what can be learned from them.

We use the [igraph package](http://igraph.org/r/), which provides the class of graphs and the basic suite of tools we build upon. The author is neither a programmer nor a computer scientist by training; any suggestions on how to make this document or the suite of functions it overviews would be most welcome.



## Case study: Southern women

In their book [*Deep South*](http://books.google.com/books?id=Q3b9QTOgLFcC), two couples of social anthropologists presented a comprehensive case study of the American caste system as it operated in a rural southern town. Among their records were several tables of coattendance at distinct events by groups of acquainted women. One of these, labeled Clique A (p. 209, Fig. 11) consists of five women, designated "Miss A" through "Miss E", and five events, which we refer to as bridge, dinner, movies, dance, and visiting, each of which was attended by a subset of the women. The attendance table is the basis for the graph object "women" included in the package:


```r
data(women, package = "bitriad")
DDGG2 <- women
```

Since the graph is bipartite, we can get all the incidence information we need from one corner of the full adjacency matrix. Due to the structure of the file and the import method, the actor nodes are listed first and the event nodes second:


```r
get.incidence(DDGG2)
```

```
##        Bridge Dinner Movies Dance Visiting
## Miss A      1      0      1     1        0
## Miss B      0      0      1     1        0
## Miss C      1      1      0     0        1
## Miss D      1      1      1     0        0
## Miss E      0      1      0     1        1
```

### Visualization

First let's visualize the network, arranging the nodes using the Fruchterman-Reingold algorithm:

<img src="figure/unnamed-chunk-5.png" title="plot of chunk unnamed-chunk-5" alt="plot of chunk unnamed-chunk-5" style="display: block; margin: auto;" />

This layout reveals a symmetry of the network between its actor and event nodes: Exchanging Miss A and Event 2, Miss B and Event 5, and so on yields a graph isomorphism. Thus any structural information we learn about the actors in this network can be flipped into equivalent information about the events. While this equivalence is unusual, the structural *duality* between actors and events, which allows us to use actor-centric tools in the study of events (and vice-versa), is of central importance to the study of affiliation networks.

### Triad census

This social network is just large enough exhibit a diversity of triads and just small enough to allow us to examine them in detail. (Future networks will only be examined cursorily or through statistics.)

Classically, the *triad census* refers to the distribution of triads of 16 isomorphism classes throughout a simple, directed network. The women's clique is neither simple nor directed, but we can view a simplified (undirected) version of the triad census on its *one-mode projection*---the network of women with acquaintanceship ties inferred from their coattendence at events. There are only four isomorphism classes of undirected triads, distinguished by the number of edges (0 through 3) among the three nodes:


```r
DDGG2.proj <- onemode.projection(DDGG2)
stc <- simple.triad.census(DDGG2.proj)
stc
```

```
## [1] 0 0 3 7
```

There are no disconnected triples among the women, only three 'wedges' or 'vees' and seven 'triangles'. These probably exhibit some diversity of their own that is lost in the projection. To find  out, we can take a look at the **(full) two-mode triad census**. Given an affiliation network, this census tallies all triples of actors by how they coattend events---the number of events they all three attended and the distribution among them of events only attended by two. (Events only attended by one member of a triad do not serve to link them so are omitted from consideration.)


```r
tmtc <- twomode.triad.census(DDGG2)
tmtc
```

```
##       [,1] [,2]
##  [1,]    0    0
##  [2,]    0    1
##  [3,]    0    3
##  [4,]    1    0
##  [5,]    0    0
##  [6,]    3    0
##  [7,]    2    0
##  [8,]    0    0
##  [9,]    0    0
## [10,]    0    0
```

The arrangement is less intuitive than that of the simple census. The rows are labeled according to the partition ( x ≥ y ≥ z ) formed from the number of events coattended by each pair of women in a triad but not the other; for instance, Miss A and Miss B attended two events (movies and dance) without Miss C, and Miss A and Miss C attended one event (bridge) without Miss B, while Miss B and Miss C attended no events together. Thus the triad (A, B, C) is tallied in the sixth row of the census, labeled by the partition (2 ≥ 1 ≥ 0). We already observed that Miss B and Miss C attended no events together at all---even without Miss A. Therefore not only is the third part of the partition zero, but so is the value of w, the "triad weight" that indexes the columns of the census. The triad is identified by this pair of objects (pairwise partition and triad weight): ( ( 2 ≥ 1 ≥ 0 ), 0 ).

We can sacrifice information about event multiplicity within triads for a simpler and more intuitive layout. This alternative is dubbed the **cooperative triad census** because it collapses the events to only four types, based on which actors cooperated on them, or coattended them:


```r
ctc <- tmtc2ctc(tmtc)
ctc
```

```
##      [,1] [,2]
## [1,]    0    0
## [2,]    0    1
## [3,]    3    3
## [4,]    3    0
```

The column indicates the existence of a triadwise event; the row indicates the number of pairs of actors connected by a pairwise event (0, 1, 2, or 3). The simple triad census can also be recovered from either the full or the cooperative two-mode census:


```r
ctc2stc(ctc)
```

```
## [1] 0 0 3 7
```

### Global clustering coefficients

The classical (global) clustering coefficient for a one-mode network may be defined either as the proportion of "wedges" that are "closed" or as the ratio of (three times) the number of "triangles" to the number of "wedges". Here wedges are 2-paths, distinguished by the relative positions of the nodes but not by their progression, and a wedge is considered closed if its end nodes are tied. (To avoid confusion i won't get into the other definition.) Since every triad of three edges counts thrice as a closed wedge, we can compute the clustering coefficient of the one-mode projection directly from the simple census:


```r
C <- 3 * stc[4]/(stc[3] + 3 * stc[4])
C
```

```
## [1] 0.875
```

The value tells us what proportion of the time each pair of three women have co-attended at least one event, given that two pairs have. (Note that this is a different value from the proportion of the time that two women have co-attended an event, given that they have at least one common co-attendee between them.) The clustering coefficient has proven a valuable, though heavily biased, single-value indicator of transitivity---the tendency for near-connections to indicate direct connections, or for "friends of friends" to in fact be "friends".

The paper discusses in detail two alternative clustering coefficients specifically designed for two-mode networks. The first of these is the *[Opsahl](http://toreopsahl.com/2011/12/21/article-triadic-closure-in-two-mode-networks-redefining-the-global-and-local-clustering-coefficients/) clustering coefficient*, the first proposal for a truly two-mode measure of transitive linking. The second is dubbed the **exclusive clustering coefficient** because it depends only on the existence, and not the number, of pairwise-exclusive events for each pair of actors. Analogously to the above relationship, each of these diagnostics is recoverable from the two-mode triad census, which is how they are calculated below. (Because the number of events of each type does not matter to it, the exclusive clustering coefficient can be computed from the cooperativity triad census as well as directly from the full census.)


```r
global.c1 <- c(C = tmtc2C(tmtc), C.O = tmtc2CO(tmtc), C.X = tmtc2CX(tmtc))
global.c1
```

```
##      C    C.O    C.X 
## 0.8750 0.6111 0.6000
```

### Local clustering coefficients

So far we have only tried to gauge transitivity tendencies in the network as a whole; that is, we have been looking at global network properties. But triadic analysis has always taken place at two levels---the micro and the macro: The Davis/Holland/Leinhardt studies tested macro network properties through their micro predictions, and the global clustering coefficient was a macro counterpart to the original (local) clustering coefficient of Watts and Strogatz. Having viewed the southern women through this global lens, we now turn to the local.

The classical local clustering coeffiicent at a node Q is the proportion of pairs of neighbors of Q who are themselves neighobrs. From the images above we can see that the only pair of women not linked through at least one event are Miss B and Miss C. This means that the only local clustering coefficients we'll observe are 5/6 (for women who count Miss B and Miss C among their neighobrs) and 1 (for Miss B and Miss C). To verify, we specify the type to 'local' in the base igraph function:


```r
local.c <- transitivity(DDGG2.proj, type = "local")
local.c
```

```
## [1] 0.8333 1.0000 1.0000 0.8333 0.8333
```

Our two-mode-sensitive candidates are implemented using a 'twomode.transitivity' shell with a required function that counts the open and closed wedges at each node. This "wedge function" determines the species of two-mode transitivity that will be calculated. The shell uses the wedges to compute the corresponding local or global clustering coefficient, or (if neither is specified) returns the wedge list itself.


```r
local.c.df <- cbind(C = local.c, C.O = opsahl.transitivity(DDGG2, type = "local"), 
    C.X = excl.transitivity(DDGG2, type = "local"))
rownames(local.c.df) <- V(DDGG2.proj)$name
local.c.df
```

```
##             C    C.O  C.X
## Miss A 0.8333 0.5000 0.50
## Miss B 1.0000 0.6667 1.00
## Miss C 1.0000 0.6667 0.50
## Miss D 0.8333 0.6000 0.50
## Miss E 0.8333 0.7143 0.75
```

As a reality check, we can test the 'global' option for type of these implementations against the global values produced from the two-mode triad census.


```r
global.c2 <- c(transitivity(DDGG2.proj), opsahl.transitivity(DDGG2), excl.transitivity(DDGG2))
data.frame(From.census = global.c1, From.wedges = global.c2)
```

```
##     From.census From.wedges
## C        0.8750      0.8750
## C.O      0.6111      0.6111
## C.X      0.6000      0.6000
```

### Wedge-dependent local clustering

One [thoroughly documented](http://arxiv.org/abs/cond-mat/0211528) property of social networks is the inverse relationship between local connectivity and local clusterability. This relationship can be repackaged as one between the *potential* for clustering at a node Q, given by the number of 2-paths through Q (this number is k(k - 1)/2, or "k choose 2", when Q has degree k), and the *actual* clustering at Q, given as the local clustering coefficient.

The typical analysis plots the mean "degree-dependent" local clustering coefficient, taken over all nodes of a fixed degree, against the degree. The framework also prompts a question i have not yet found answered in the literature: For a fixed degree k, what does the distribution of local clustering coefficients at nodes of degree k look like? The assumption underlying the typical analysis is that the mean of this distribution is a reasonable one-variable summary of it, but the possibility exists that these distributions are skewed or otherwise asymmetric.

While Clique A is too small to draw general inferences from, it can at least provide a case study and a demonstration of these diagnostics. Since the "degree" and "transitivity" functions (the latter using the 'local' value of type) are evaluated at the nodes in order of their IDs, we can match them up in a simple data frame:


```r
ddc <- data.frame(k = degree(DDGG2.proj), C = transitivity(DDGG2.proj, type = "local"))
print(ddc)
```

```
##        k      C
## Miss A 4 0.8333
## Miss B 3 1.0000
## Miss C 3 1.0000
## Miss D 4 0.8333
## Miss E 4 0.8333
```

As we observed above, there is zero variability among nodes of common degree, though we can still plot the relationship between the (trivial) degree-dependent mean local clustering coefficients and the degrees:


```r
plot(aggregate(ddc$C, by = list(ddc$k), FUN = mean), pch = 19, type = "b", main = "Degree-dependent local clustering", 
    xlab = "Degree", ylab = "Mean conditional local clustering coefficient")
```

<img src="figure/unnamed-chunk-16.png" title="plot of chunk unnamed-chunk-16" alt="plot of chunk unnamed-chunk-16" style="display: block; margin: auto;" />

Though the curve at least proceeds in the expected direction, there is little insight to be gleaned here. A more heterogeneous network is required. Fortunately for us, another, somewhat larger (but still manageable) table of women and events is available to us, labeled Group I (p. 148). The data are available [here] [1] in R format and were constructed (with one minor correction) from data at [Tore Opsahl's site] [2].

[1]: https://github.com/corybrunson/bitriad/tree/master/data
[2]: http://toreopsahl.com/datasets/#southernwomen



Again let's begin with a plot:

<img src="figure/unnamed-chunk-18.png" title="plot of chunk unnamed-chunk-18" alt="plot of chunk unnamed-chunk-18" style="display: block; margin: auto;" />

The visualization is quite a bit messier, but it looks like we have at least some range of degrees this time:


```r
DDGG1.proj <- onemode.projection(DDGG1)
ddc2 <- data.frame(k = degree(DDGG1.proj), C = transitivity(DDGG1.proj, type = "local"))
print(ddc2)
```

```
##            k      C
## Evelyn    17 0.8971
## Laura     15 0.9619
## Theresa   17 0.8971
## Brenda    15 0.9619
## Charlotte 11 1.0000
## Frances   15 0.9619
## Eleanor   15 0.9619
## Pearl     16 0.9333
## Ruth      17 0.8971
## Verne     17 0.8971
## Myra      16 0.9333
## Katherine 16 0.9333
## Sylvia    17 0.8971
## Nora      17 0.8971
## Helen     17 0.8971
## Dorothy   16 0.9333
## Olivia    12 1.0000
## Flora     12 1.0000
```

```r
plot(aggregate(ddc2$C, by = list(k = ddc2$k), FUN = mean), pch = 19, type = "b", 
    main = "Degree-dependent local clustering", xlab = "Degree", ylab = "Mean conditional local clustering coefficient")
```

<img src="figure/unnamed-chunk-19.png" title="plot of chunk unnamed-chunk-19" alt="plot of chunk unnamed-chunk-19" style="display: block; margin: auto;" />

There is clearly a trade-off between the number of a woman's acquaintances (through events) and the proportion of those acquaintances that are also acquainted; perhaps one's capacity for acquaintanceship outpaces one's ability to make introductions and forge new acquaintanceships.

This distribution might be fruitfully generalized to the two-mode setting. The only chore is to come up with a suitable analog of degree---that is, a measure of local connectivity on which local clustering can be meaningfully conditioned. As suggested by the discussion above, we can adopt local wedge counts, which the twomode.transitivity function returns when neither type (local or global) is specified. Here are the wedge-dependent means and distributions using Opsahl's clustering coefficient:


```r
DDGG1.wedges <- opsahl.transitivity(DDGG1, type = "")
DDGG1.wedges <- cbind(DDGG1.wedges, C = DDGG1.wedges$T/DDGG1.wedges$V)
plot(aggregate(DDGG1.wedges$C, by = list(V = DDGG1.wedges$V), FUN = mean), pch = 19, 
    type = "b", main = "Wedge-dependent local clustering (Opsahl)", xlab = "Wedges", 
    ylab = "Mean conditional local clustering coefficient")
```

<img src="figure/unnamed-chunk-20.png" title="plot of chunk unnamed-chunk-20" alt="plot of chunk unnamed-chunk-20" style="display: block; margin: auto;" />

This plot defies the consistent behavior we saw in the classical case. What, instead, if we try exclusive clustering?


```r
DDGG1.wedges <- excl.transitivity(DDGG1, type = "")
DDGG1.wedges <- cbind(DDGG1.wedges, C = DDGG1.wedges$T/DDGG1.wedges$V)
plot(aggregate(DDGG1.wedges$C, by = list(V = DDGG1.wedges$V), FUN = mean), pch = 19, 
    type = "b", main = "Wedge-dependent local clustering (exclusive)", xlab = "Wedges", 
    ylab = "Mean conditional local clustering coefficient")
```

<img src="figure/unnamed-chunk-21.png" title="plot of chunk unnamed-chunk-21" alt="plot of chunk unnamed-chunk-21" style="display: block; margin: auto;" />

This plot recovers the steady, though not strictly monotonic, behavior of the classical case. In the classical case we expect local clustering coefficients to be quite large in tight-knit networks such as those produced for sociological analysis of cliques and communities; the exclusive clustering coefficient captures a more descriptive form of transitivity.

Still, however, at most two nodes share a wedge count; we will need [larger networks](http://link.springer.com/article/10.1007%2Fs11192-013-1209-z) in order to get a sense for the distributions of the wedge-conditioned local clustering coefficients. Stay tuned!

