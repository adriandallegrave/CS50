SELECT AVG(rating) FROM (SELECT rating, year FROM ratings JOIN movies ON ratings.movie_id = movies.id WHERE year = 2012);