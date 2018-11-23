CREATE TABLE miasta (
name varchar(40) not null,
coordinates geometry(point,4326) not null
);

INSERT INTO miasta VALUES('Krakow', ST_GeomFromText('POINT(19.938333
50.061389)', 4326));
INSERT INTO miasta VALUES(Gdansk, ST_GeomFromText('POINT(18.646370 54.352050)', 4326));

SELECT ST_DISTANCE(kr. coordinates, gd. coordinates)
FROM miasta kr, miasta gd
WHERE kr.name='Krakow' and gd.name=‘Gdansk';

SELECT ST_DISTANCE(ST_Transform(kr. coordinates,2180), ST_Transform(gd. coordinates,2180))
FROM miasta kr, miasta gd
WHERE kr.name='Krakow' and gd.name=‘Gdansk';

SELECT ST_DISTANCE(ST_Transform(kr. coordinates,3120), ST_Transform(gd. coordinates,3120))
FROM miasta kr, miasta gd
WHERE kr.name='Krakow' and gd.name=‘Gdansk';

CREATE TABLE miasta (
name varchar(40) not null,
coordinates geography(point,4326) not null
);

INSERT INTO miasta VALUES('Krakow', ST_GeomFromText('POINT(19.938333
50.061389)', 4326));
INSERT INTO miasta VALUES(Gdansk, ST_GeomFromText('POINT(18.646370 54.352050)', 4326));

SELECT ST_DISTANCE(kr. coordinates, gd. coordinates)
FROM miasta kr, miasta gd
WHERE kr.name='Krakow' and gd.name=‘Gdansk';


shp2pgsql -s 3785 admin.shp > admin.sql
Shapefile type: Polygon
	Postgis type: MULTIPOLYGON[2]

shp2pgsql -s 3785 amenities.shp > amenities.sql
	Shapefile type: Point
	Postgis type: POINT[2]


shp2pgsql -s 3785 roads.shp > roads.sql
Shapefile type: Arc
	Postgis type: MULTILINESTRING[2]

psql -f admin.sql;
psql -f amenities.sql;
psql -f roads.sql;

ALTER TABLE admin ADD geo geography(MULTIPOLYGON,4326);
ALTER TABLE
ALTER TABLE amenities ADD geo geography(POINT,4326);
ALTER TABLE
ALTER TABLE roads ADD geo geography(MULTILINESTRING,4326);
ALTER TABLE

UPDATE admin SET geography= ST_Transform(geom, 4326);
	UPDATE 1
UPDATE amenities SET geography= ST_Transform(geom, 4326);
UPDATE 421
UPDATE roads SET geography= ST_Transform(geom, 4326);
UPDATE 37318

SELECT ST_Area(ST_Transform(kr.geom,2180)) 
FROM admin kr WHERE kr.id=1;

SELECT ST_Area(kr.geography) 
FROM admin kr;

SELECT SUM(ST_LENGTH(roads.geography))
FROM admin, roads;

SELECT SUM( ST_LENGTH( roads.geography ))
FROM roads
WHERE class='motorways';

SELECT TYPE, COUNT(*)
FROM amenities
WHERE ST_DISTANCE(
ST_TRANSFORM(ST_GeomFromText('POINT(20.001932 50.085339)', 4326),
2180),
ST_TRANSFORM(amenities.geom, 2180)
) <=2000
GROUP BY TYPE
ORDER BY count DESC;

SELECT roads.name, SUM(ST_LENGTH(roads.geom))
FROM roads, admin
WHERE class!='railways' and ST_CONTAINS(admin.geom, roads.geom) and
roads.name is not null
GROUP BY roads.name
ORDER BY 2 desc
LIMIT 10;
