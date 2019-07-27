CREATE FUNCTION get_my_episodes_grouped_by_podcasts(userId text)
RETURNS SETOF podcasts_from_my_episodes AS $$
   
  (
    SELECT "numberOfEpisodes", "lastEpisodeAddedDate", "listennotesId", "title", "description", "publisher", "itunesId", "genreIds", "image", NULL as "podcastAddedDate" 
    FROM 
    (
        SELECT COUNT("episodeId") as "numberOfEpisodes", "podcastListennotesId", MAX("addedDate") as "lastEpisodeAddedDate"
        FROM my_episodes
        INNER JOIN episodes
            ON "episodeId"="listennotesId"
            AND "userId"=userId
        GROUP BY "podcastListennotesId"
    ) as podcasts_with_episodes
    JOIN podcasts
        ON "listennotesId"="podcastListennotesId"
  )
  UNION
   SELECT 0 as "numberOfEpisodes", NULL as "lastEpisodeAddedDate", "listennotesId", "title", "description", "publisher", "itunesId", "genreIds", "image", "addedDate" as "podcastAddedDate"
    FROM my_podcasts 
    INNER JOIN podcasts
        ON "listennotesId"="podcastId" 
        AND "userId"=userId 
        AND "podcastId" NOT IN 
        (
            SELECT "podcastListennotesId"
            FROM my_episodes
            INNER JOIN episodes
                ON "episodeId"="listennotesId"
                AND "userId"=userId
        )
  ORDER BY "lastEpisodeAddedDate" desc  NULLS LAST, "podcastAddedDate" desc;
$$ LANGUAGE sql STABLE
