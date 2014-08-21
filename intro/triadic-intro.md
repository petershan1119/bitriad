# Triadic analyses of two-mode networks


```r
library(knitr)
opts_knit$set(progress = FALSE)
opts_chunk$set(echo = TRUE, message = FALSE, tidy = TRUE, comment = NA,
               fig.path = "figure/", fig.keep = "high", fig.width = 10,
               fig.height = 6, fig.align = "center")
```

The paper "Triadic analysis for two-mode networks" will make a case for adopting a coherent batch of triad-centric tools for the study of two-mode, usually affiliation, networks. This RMarkdown file (view the html [here](http://htmlpreview.github.io/?https://raw.githubusercontent.com/corybrunson/triadic/master/intro/triadic-intro.html)) will apply these tools to the study of several manageably-sized real-world affiliation networks, in hopes of giving the reader a feel for what they mean, how they can be used, and what can be learned from them.

We use the "igraph" package, which provides the class of graphs and the basic suite of tools we build upon. We'll also read data and functions from the author's github account; the custom functions below make use of the function 'source_https' is taken from [tonybreyal](http://tonybreyal.wordpress.com/2011/11/24/source_https-sourcing-an-r-script-from-github/ "tonybreyal").



The author is neither a programmer nor a computer scientist by training; any suggestions on how to make this document or the suite of functions it overviews would be most welcome.

## Case study: Southern women

In their book [*Deep South*](http://books.google.com/books?id=Q3b9QTOgLFcC), two couples of social anthropologists presented a comprehensive case study of the American caste system as it operated in a rural southern town. Among their records were several tables of coattendance at distinct events by groups of acquainted women. One of these, labeled Clique A (p. 209, Fig. 11) consists of five women, designated "Miss A" through "Miss E", and five events, which we refer to as bridge, dinner, movies, dance, and visiting, each of which was attended by a subset of the women. The attendance table is reproduced in the file "DGG\_Clique\_A.csv", which we load into R directly as a graph:


```r
ddgg <- graph.incidence(as.matrix(mycsv("DGG-CliqueA.csv", row.names = 1)))
```

Since the graph is bipartite, we can get all the incidence information we need from one corner of the full adjacency matrix. Due to the structure of the file and the import method, the actor nodes are listed first and the event nodes second:


```r
get.incidence(ddgg)
```

```
       Bridge Dinner Movies Dance Visiting
Miss A      1      0      1     1        0
Miss B      0      0      1     1        0
Miss C      1      1      0     0        1
Miss D      1      1      1     0        0
Miss E      0      1      0     1        1
```

### Visualization

First let's visualize the network, arranging the nodes using the Fruchterman-Reingold algorithm:


```r
set.seed(10)
plot(ddgg, layout = layout.fruchterman.reingold(ddgg, niter = 100), vertex.color = ifelse(V(ddgg)$type == 
    0, "SkyBlue2", "lightcoral"), vertex.shape = ifelse(V(ddgg)$type == 0, "circle", 
    "square"), edge.width = 2, edge.color = "black", vertex.label = c(LETTERS[1:5], 
    1:5), vertex.label.family = "sans", vertex.label.color = "white")
```

<img src="figure/unnamed-chunk-5.png" title="plot of chunk unnamed-chunk-5" alt="plot of chunk unnamed-chunk-5" style="display: block; margin: auto;" />

This layout reveals a symmetry of the network between its actor and event nodes: Exchanging Miss A and Event 2, Miss B and Event 5, and so on yields a graph isomorphism. Thus any structural information we learn about the actors in this network can be flipped into equivalent information about the events.

### Triad census

This social network is just large enough exhibit a diversity of triads and just small enough to allow us to examine them in detail. (Future networks will only be examined cursorily or through statistics.) We can view the (undirected) triad census of the one-mode projection, taking the actor-based one-mode projection:


```r
ddgg.proj <- triadic::onemode.projection(ddgg)
stc <- triadic::simple.triad.census(ddgg.proj)
stc
```

```
0 1 2 3 
0 0 3 7 
```

We have no disconnected triples, only three 'wedges' or 'vees' and seven 'triangles'. But these probably exhibit some diversity of their own that is lost in the projection. We can take a look at the two-mode triad census using the function 'twomode.triad.census':


```r
tmtc <- triadic::twomode.triad.census(ddgg)
tmtc
```

```
      [,1] [,2]
 [1,]    0    0
 [2,]    0    1
 [3,]    0    3
 [4,]    1    0
 [5,]    0    0
 [6,]    3    0
 [7,]    2    0
 [8,]    0    0
 [9,]    0    0
[10,]    0    0
```

The arrangement is far less intuitive than that of the simple census. The rows are labeled according to the partition ( x ≥ y ≥ z ) formed from the number of events coattended by each pair of women in a triad but not the other; for instance, Miss A and Miss B attended two events (movies and dance) without Miss C, and Miss A and Miss C attended one event (bridge) without Miss B, while Miss B and Miss C attended no events together. Thus the triad (A, B, C) is tallied in the sixth row of the census, labeled by the partition (2 ≥ 1 ≥ 0). We already observed that Miss B and Miss C attended no events together at all---even without Miss A. Therefore not only is the third part of the partition zero, but so is the value of w, the "triad weight" that indexes the columns of the census. The triad is identified by this pair of objects (pairwise partition and triad weight): ( ( 2 ≥ 1 ≥ 0 ), 0 ).

We can sacrifice information about event multiplicity within triads for a simpler and more intuitive layout. This alternative is dubbed the structural triad census because it is obtained by taking the quotient of the full triad census by structural equivalence of events:


```r
setc <- triadic::tmtc2setc(tmtc)
setc
```

```
     [,1] [,2]
[1,]    0    0
[2,]    0    1
[3,]    3    3
[4,]    3    0
```

The column indicates the existence of a triadwise event; the row indicates the number of pairs of actors connected by a pairwise event (0, 1, 2, or 3).

### Global clustering coefficients

The classical (global) clustering coefficient for a one-mode network may be defined either as the proportion of "wedges" that are "closed" or as the ratio of (three times) the number of "triangles" to the number of "wedges". Here wedges are 2-paths, distinguished by the relative positions of the nodes but not by their progression, and a wedge is considered closed if its end nodes are tied. (To avoid confusion i won't get into the other definition.) Since every triad of three edges counts thrice as a closed wedge, we can compute the clustering coefficient of the one-mode projection directly from the simple census:


```r
C <- 3 * stc[4]/(stc[3] + 3 * stc[4])
C
```

```
    3 
0.875 
```

The value tells us what proportion of the time each pair of three women have co-attended at least one event, given that two pairs have. (Note that this is a different value from the proportion of the time that two women have co-attended an event, given that they have at least one common co-attendee between them.) The clustering coefficient has proven a valuable, though heavily biased, single-value indicator of transitivity---the tendency for near-connections to indicate direct connections, or for "friends of friends" to in fact be "friends".

Naturally, this diagnostic can also be recovered from the two-mode census; for this and other recoveries we call a suite of functions written for the purpose:


```r
triadic::tmtc2C(tmtc)
```

```
[1] 0.875
```

The paper discusses in detail two alternative clustering coefficients specifically designed for two-mode networks. The first of these is the *[Opsahl](http://toreopsahl.com/2011/12/21/article-triadic-closure-in-two-mode-networks-redefining-the-global-and-local-clustering-coefficients/) clustering coefficient*:


```r
triadic::tmtc2CO(tmtc)
```

```
[1] 0.6111
```

The second is dubbed the *exclusive clustering coefficient* because it depends only on the existence, and not the number, of pairwise-exclusive events for each pair of actors. Because the number does not matter, the exclusive clustering coefficient can also be computed from the structural triad census analogously to C:


```r
triadic::tmtc2Cex(tmtc)
```

```
[1] 0.6
```

```r
3 * sum(setc[4, ])/(sum(setc[3, ]) + 3 * sum(setc[4, ]))
```

```
[1] 0.6
```

Here's how the three diagnostics line up:


```r
global.c1 <- c(C = triadic::tmtc2C(tmtc), C.O = triadic::tmtc2CO(tmtc), C.X = triadic::tmtc2Cex(tmtc))
global.c1
```

```
     C    C.O    C.X 
0.8750 0.6111 0.6000 
```

### Local clustering coefficients

So far we have only tried to gauge transitivity tendencies in the network as a whole; that is, we have been looking at global network properties. But triadic analysis has always taken place at two levels---the micro and the macro: The Davis/Holland/Leinhardt studies tested macro network properties through their micro predictions, and the global clustering coefficient was a macro counterpart to the original (local) clustering coefficient of Watts and Strogatz. Having viewed the southern women through this global lens, we now turn to the local.

The classical local clustering coeffiicent at a node Q is the proportion of pairs of neighbors of Q who are themselves neighobrs. From the images above we can see that the only pair of women not linked through at least one event are Miss B and Miss C. This means that the only local clustering coefficients we'll observe are 5/6 (for women who count Miss B and Miss C among their neighobrs) and 1 (for Miss B and Miss C). To verify, we specify the type to 'local' in the base igraph function:


```r
local.c <- transitivity(ddgg.proj, type = "local")
local.c
```

```
[1] 0.8333 1.0000 1.0000 0.8333 0.8333
```

Our two-mode-sensitive candidates, as implemented independently (rather than through the two-mode triad census) are specialized with a similar local option for type:


```r
local.c.df <- cbind(c = local.c, c.O = triadic::opsahl.transitivity(ddgg, type = "local"), 
    c.X = triadic::excl.transitivity(ddgg, type = "local"))
rownames(local.c.df) <- V(ddgg.proj)$name
local.c.df
```

```
            c    c.O  c.X
Miss A 0.8333 0.5000 0.50
Miss B 1.0000 0.6667 1.00
Miss C 1.0000 0.6667 0.50
Miss D 0.8333 0.6000 0.50
Miss E 0.8333 0.7143 0.75
```

As a reality check, we can test the 'global' option for type of these implementations against the global values produced from the two-mode triad census.


```r
global.c2 <- c(transitivity(ddgg.proj), triadic::opsahl.transitivity(ddgg), 
    triadic::excl.transitivity(ddgg))
data.frame(Census = global.c1, Separate = global.c2)
```

```
    Census Separate
C   0.8750   0.8750
C.O 0.6111   0.6111
C.X 0.6000   0.6000
```

### Wedge-dependent local clustering

One [thoroughly documented](http://arxiv.org/abs/cond-mat/0211528) property of social networks is the inverse relationship between local connectivity and local clusterability. This relationship can be repackaged as one between the *potential* for clustering at a node Q, given by the number of 2-paths through Q (this number is k(k - 1)/2, or "k choose 2", when Q has degree k), and the *actual* clustering at Q, given as the local clustering coefficient.

The typical analysis plots the mean "degree-dependent" local clustering coefficient, taken over all nodes of a fixed degree, against the degree. The framework also prompts a question i have not yet found answered in the literature: For a fixed degree k, what does the distribution of local clustering coefficients at nodes of degree k look like? The assumption underlying the typical analysis is that the mean of this distribution is a reasonable one-variable summary of it, but the possibility exists that these distributions are skewed or otherwise asymmetric.

While Clique A is too small to draw general inferences from, it can at least provide a case study and a demonstration of these diagnostics. Since the "degree" and "transitivity" functions (the latter using the 'local' value of type) are evaluated at the nodes in order of their IDs, we can match them up in a simple data frame:


```r
ddc <- data.frame(k = degree(ddgg.proj), c = transitivity(ddgg.proj, type = "local"))
print(ddc)
```

```
       k      c
Miss A 4 0.8333
Miss B 3 1.0000
Miss C 3 1.0000
Miss D 4 0.8333
Miss E 4 0.8333
```

As we observed above, there is zero variability among nodes of common degree, though we can still plot the relationship between the (trivial) degree-dependent mean local clustering coefficients and the degrees:


```r
plot(aggregate(ddc$c, by = list(ddc$k), FUN = mean), pch = 19, type = "b", main = "Degree-dependent local clustering", 
    xlab = "Degree", ylab = "Mean conditional local clustering coefficient")
```

<img src="figure/unnamed-chunk-18.png" title="plot of chunk unnamed-chunk-18" alt="plot of chunk unnamed-chunk-18" style="display: block; margin: auto;" />

Though the curve at least proceeds in the expected direction, there is little insight to be gleaned here. A more heterogeneous network is required. Fortunately for us, another, somewhat larger (but still manageable) table of women and events is available to us, labeled Group I (p. ???, Fig. ?). The data are available [here](), in a different format from the previous data (hence the different procedure to read it):


```r
data <- mytable("Davis_southern_club_women-two_mode.txt", colClasses = "numeric")
names <- mytable("Davis_southern_club_women-name.txt", colClasses = "character")
ddgg2 <- graph.data.frame(data.frame(woman = names[data[, 1], 1], event = data[, 
    2]), directed = FALSE)
V(ddgg2)$type <- !(substr(V(ddgg2)$name, 1, 1) %in% LETTERS)
```

Again let's begin with a plot:


```r
set.seed(1)
plot(ddgg2, layout = layout.fruchterman.reingold(ddgg2, niter = 100), vertex.color = ifelse(V(ddgg2)$type == 
    0, "SkyBlue2", "lightcoral"), vertex.shape = ifelse(V(ddgg2)$type == 0, 
    "circle", "square"), edge.width = 2, edge.color = "black", vertex.label = substr(V(ddgg2)$name, 
    1, 2), vertex.label.family = "sans", vertex.label.color = "white")
```

<img src="figure/unnamed-chunk-20.png" title="plot of chunk unnamed-chunk-20" alt="plot of chunk unnamed-chunk-20" style="display: block; margin: auto;" />

The visualization is quite a bit messier, but it looks like we have at least some range of degrees this time:


```r
ddgg2.proj <- triadic::onemode.projection(ddgg2)
ddc2 <- data.frame(k = degree(ddgg2.proj), c = transitivity(ddgg2.proj, type = "local"))
print(ddc2)
```

```
           k      c
EVELYN    17 0.8971
LAURA     15 0.9619
THERESA   17 0.8971
BRENDA    15 0.9619
CHARLOTTE 11 1.0000
FRANCES   15 0.9619
ELEANOR   15 0.9619
PEARL     16 0.9333
RUTH      17 0.8971
VERNE     17 0.8971
MYRA      16 0.9333
KATHERINE 16 0.9333
SYLVIA    17 0.8971
NORA      17 0.8971
HELEN     17 0.8971
DOROTHY   16 0.9333
OLIVIA    12 1.0000
FLORA     12 1.0000
```

```r
plot(aggregate(ddc2$c, by = list(k = ddc2$k), FUN = mean), pch = 19, type = "b", 
    main = "Degree-dependent local clustering", xlab = "Degree", ylab = "Mean conditional local clustering coefficient")
```

<img src="figure/unnamed-chunk-21.png" title="plot of chunk unnamed-chunk-21" alt="plot of chunk unnamed-chunk-21" style="display: block; margin: auto;" />

There is clearly a trade-off between the number of a woman's acquaintances (through events) and the proportion of those acquaintances that are also acquainted; perhaps one's capacity for acquaintanceship outpaces one's ability to make introductions and forge new acquaintanceships.

This distribution might be fruitfully generalized to the two-mode setting. The only chore is to come up with a suitable analog of degree---that is, a measure of local connectivity on which local clustering can be meaningfully conditioned. As suggested by the discussion above, we can adopt local wedge counts, which the twomode.transitivity function returns when neither type (local or global) is specified. Here are the wedge-dependent means and distributions using Opsahl's clustering coefficient:


```r
ddgg2.wedges <- triadic::opsahl.transitivity(ddgg2, type = "")
ddgg2.wedges <- cbind(ddgg2.wedges, C = ddgg2.wedges$T/ddgg2.wedges$V)
plot(aggregate(ddgg2.wedges$C, by = list(V = ddgg2.wedges$V), FUN = mean), pch = 19, 
    type = "b", main = "Wedge-dependent local clustering (Opsahl)", xlab = "Wedges", 
    ylab = "Mean conditional local clustering coefficient")
```

<img src="figure/unnamed-chunk-22.png" title="plot of chunk unnamed-chunk-22" alt="plot of chunk unnamed-chunk-22" style="display: block; margin: auto;" />

This plot defies the consistent behavior we saw in the classical case. What, instead, if we try exclusive clustering?


```r
ddgg2.wedges <- triadic::excl.transitivity(ddgg2, type = "")
ddgg2.wedges <- cbind(ddgg2.wedges, C = ddgg2.wedges$T/ddgg2.wedges$V)
plot(aggregate(ddgg2.wedges$C, by = list(V = ddgg2.wedges$V), FUN = mean), pch = 19, 
    type = "b", main = "Wedge-dependent local clustering (exclusive)", xlab = "Wedges", 
    ylab = "Mean conditional local clustering coefficient")
```

<img src="figure/unnamed-chunk-23.png" title="plot of chunk unnamed-chunk-23" alt="plot of chunk unnamed-chunk-23" style="display: block; margin: auto;" />

This plot recovers the steady, though not strictly monotonic, behavior of the classical case. In the classical case we expect local clustering coefficients to be quite large in tight-knit networks such as those produced for sociological analysis of cliques and communities; the exclusive clustering coefficient captures a more descriptive form of transitivity.

Still, however, at most two nodes share a wedge count; we will need [larger networks](http://link.springer.com/article/10.1007%2Fs11192-013-1209-z) in order to get a sense for the distributions of the wedge-conditioned local clustering coefficients. Stay tuned!