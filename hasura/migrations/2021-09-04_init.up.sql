CREATE TABLE podcasts_from_my_episodes (
    "numberOfEpisodes" bigint NOT NULL,
    "lastEpisodeAddedDate" timestamp with time zone,
    id text NOT NULL,
    title text NOT NULL,
    description text NOT NULL,
    publisher text NOT NULL,
    "itunesId" text NOT NULL,
    "genreIds" text NOT NULL,
    image text NOT NULL,
    "podcastAddedDate" timestamp with time zone
);

CREATE TABLE episodes (
    id text NOT NULL,
    "itunesId" text,
    "podcastId" text NOT NULL,
    title text NOT NULL,
    description text NOT NULL,
    "lengthSec" integer NOT NULL,
    "pubDate" text NOT NULL
);
CREATE TABLE my_episodes (
    "userId" text NOT NULL,
    "episodeId" text NOT NULL,
    status text NOT NULL,
    tags text NOT NULL,
    "addedDate" timestamp with time zone DEFAULT now()
);
CREATE TABLE my_podcasts (
    "userId" text NOT NULL,
    "podcastId" text NOT NULL,
    tags text NOT NULL,
    "addedDate" timestamp with time zone DEFAULT now() NOT NULL
);
CREATE TABLE podcasts (
    id text NOT NULL,
    title text NOT NULL,
    description text NOT NULL,
    publisher text NOT NULL,
    "itunesId" text NOT NULL,
    "genreIds" text NOT NULL,
    image text NOT NULL
);
ALTER TABLE ONLY episodes
    ADD CONSTRAINT episodes_listennotes_id_key UNIQUE (id);
ALTER TABLE ONLY episodes
    ADD CONSTRAINT episodes_pkey PRIMARY KEY (id);
ALTER TABLE ONLY my_episodes
    ADD CONSTRAINT my_episodes_pkey PRIMARY KEY ("userId", "episodeId");
ALTER TABLE ONLY my_podcasts
    ADD CONSTRAINT my_podcasts_pkey PRIMARY KEY ("podcastId", tags);
ALTER TABLE ONLY podcasts_from_my_episodes
    ADD CONSTRAINT "podcasts_from_my_episodes_listennotesId_key" UNIQUE (id);
ALTER TABLE ONLY podcasts_from_my_episodes
    ADD CONSTRAINT podcasts_from_my_episodes_pkey PRIMARY KEY (id);
ALTER TABLE ONLY podcasts
    ADD CONSTRAINT "podcasts_itunesId_key" UNIQUE ("itunesId");
ALTER TABLE ONLY podcasts
    ADD CONSTRAINT "podcasts_listennotesId_key" UNIQUE (id);
ALTER TABLE ONLY podcasts
    ADD CONSTRAINT podcasts_pkey PRIMARY KEY (id);
ALTER TABLE ONLY episodes
    ADD CONSTRAINT "episodes_podcastListennotesId_fkey" FOREIGN KEY ("podcastId") REFERENCES podcasts(id);
ALTER TABLE ONLY my_episodes
    ADD CONSTRAINT "my_episodes_episodeId_fkey" FOREIGN KEY ("episodeId") REFERENCES episodes(id) ON UPDATE RESTRICT ON DELETE RESTRICT;
ALTER TABLE ONLY my_podcasts
    ADD CONSTRAINT "my_podcasts_podcastId_fkey" FOREIGN KEY ("podcastId") REFERENCES podcasts(id) ON UPDATE RESTRICT ON DELETE RESTRICT;

CREATE FUNCTION get_my_episodes_grouped_by_podcasts(userid text) RETURNS SETOF podcasts_from_my_episodes
    LANGUAGE sql STABLE
    AS $$
  (
    SELECT "numberOfEpisodes", "lastEpisodeAddedDate", "id", "title", "description", "publisher", "itunesId", "genreIds", "image", NULL as "podcastAddedDate" 
    FROM 
    (
        SELECT COUNT("episodeId") as "numberOfEpisodes", "podcastId", MAX("addedDate") as "lastEpisodeAddedDate"
        FROM my_episodes
        INNER JOIN episodes
            ON "episodeId"="id"
            AND "userId"=userId
        GROUP BY "podcastId"
    ) as podcasts_with_episodes
    JOIN podcasts
        ON "id"="podcastId"
  )
  UNION
   SELECT 0 as "numberOfEpisodes", NULL as "lastEpisodeAddedDate", "id", "title", "description", "publisher", "itunesId", "genreIds", "image", "addedDate" as "podcastAddedDate"
    FROM my_podcasts 
    INNER JOIN podcasts
        ON "id"="podcastId" 
        AND "userId"=userId 
        AND "podcastId" NOT IN 
        (
            SELECT "podcastId"
            FROM my_episodes
            INNER JOIN episodes
                ON "episodeId"="id"
                AND "userId"=userId
        )
  ORDER BY "lastEpisodeAddedDate" desc  NULLS LAST, "podcastAddedDate" desc;
$$;
