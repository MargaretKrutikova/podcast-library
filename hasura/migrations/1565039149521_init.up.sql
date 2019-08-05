SET xmloption = content;
CREATE TABLE public.podcasts_from_my_episodes (
    "numberOfEpisodes" bigint NOT NULL,
    "lastEpisodeAddedDate" timestamp with time zone,
    "listennotesId" text NOT NULL,
    title text NOT NULL,
    description text NOT NULL,
    publisher text NOT NULL,
    "itunesId" text NOT NULL,
    "genreIds" text NOT NULL,
    image text NOT NULL,
    "podcastAddedDate" timestamp with time zone
);

CREATE TABLE public.episodes (
    "listennotesId" text NOT NULL,
    "itunesId" text,
    "podcastListennotesId" text NOT NULL,
    title text NOT NULL,
    description text NOT NULL,
    "lengthSec" integer NOT NULL,
    "pubDate" text NOT NULL
);
CREATE TABLE public.my_episodes (
    "userId" text NOT NULL,
    "episodeId" text NOT NULL,
    status text NOT NULL,
    tags text NOT NULL,
    "addedDate" timestamp with time zone DEFAULT now()
);
CREATE TABLE public.my_podcasts (
    "userId" text NOT NULL,
    "podcastId" text NOT NULL,
    tags text NOT NULL,
    "addedDate" timestamp with time zone DEFAULT now() NOT NULL
);
CREATE TABLE public.podcasts (
    "listennotesId" text NOT NULL,
    title text NOT NULL,
    description text NOT NULL,
    publisher text NOT NULL,
    "itunesId" text NOT NULL,
    "genreIds" text NOT NULL,
    image text NOT NULL
);
ALTER TABLE ONLY public.episodes
    ADD CONSTRAINT episodes_listennotes_id_key UNIQUE ("listennotesId");
ALTER TABLE ONLY public.episodes
    ADD CONSTRAINT episodes_pkey PRIMARY KEY ("listennotesId");
ALTER TABLE ONLY public.my_episodes
    ADD CONSTRAINT my_episodes_pkey PRIMARY KEY ("userId", "episodeId");
ALTER TABLE ONLY public.my_podcasts
    ADD CONSTRAINT my_podcasts_pkey PRIMARY KEY ("podcastId", tags);
ALTER TABLE ONLY public.podcasts_from_my_episodes
    ADD CONSTRAINT podcasts_from_my_episodes_pkey PRIMARY KEY ("listennotesId");
ALTER TABLE ONLY public.podcasts
    ADD CONSTRAINT "podcasts_itunesId_key" UNIQUE ("itunesId");
ALTER TABLE ONLY public.podcasts
    ADD CONSTRAINT podcasts_pkey PRIMARY KEY ("listennotesId");
ALTER TABLE ONLY public.episodes
    ADD CONSTRAINT "episodes_podcastListennotesId_fkey" FOREIGN KEY ("podcastListennotesId") REFERENCES public.podcasts("listennotesId");
ALTER TABLE ONLY public.my_episodes
    ADD CONSTRAINT "my_episodes_episodeId_fkey" FOREIGN KEY ("episodeId") REFERENCES public.episodes("listennotesId") ON UPDATE RESTRICT ON DELETE RESTRICT;
ALTER TABLE ONLY public.my_podcasts
    ADD CONSTRAINT "my_podcasts_podcastId_fkey" FOREIGN KEY ("podcastId") REFERENCES public.podcasts("listennotesId") ON UPDATE RESTRICT ON DELETE RESTRICT;


CREATE FUNCTION public.get_my_episodes_grouped_by_podcasts(userid text) RETURNS SETOF public.podcasts_from_my_episodes
    LANGUAGE sql STABLE
    AS $$
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
$$;
