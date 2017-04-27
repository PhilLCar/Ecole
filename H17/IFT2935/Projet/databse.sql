-- SEQUENCES
DROP SEQUENCE	seq_util;
DROP SEQUENCE	seq_obj;

CREATE SEQUENCE seq_util
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_obj
START WITH 1
INCREMENT BY 1;


-- TABLES
DROP TABLE	AUTEUR;
DROP TABLE	LIVRE;
DROP TABLE	NOM_ARTISTE;
DROP TABLE	ART;
DROP TABLE	BIJOUX;
DROP TABLE	APPAREIL_ELECTRONIQUE;
DROP TABLE	ELECTROMENAGER;
DROP TABLE	VETEMENT;
DROP TABLE	NOURRITURE;
DROP TABLE	MAISON;
DROP TABLE	MEUBLE;
DROP TABLE	MUSIQUE;
DROP TABLE	AUTOMOBILE;
DROP TABLE	JEUX;
DROP TABLE	PIECE_MECANIQUE;
DROP TABLE	TELEPHONE;
DROP TABLE	MARQUE;
DROP TABLE	INTERESSE_A;
DROP TABLE	ANNONCE;
DROP TABLE	MESSAGERIE;
DROP TABLE	PATH_PHOTO_O;
DROP TABLE	OBJET;
DROP TABLE	TYPE_OBJ;
DROP TABLE	UTILISATEUR;

--------------------------------------------------------------------------------
CREATE TABLE UTILISATEUR (
       Id_U  		 NUMBER (5)		NOT NULL	PRIMARY KEY, 
       Prenom 		 VARCHAR2 (15)   	NOT NULL,
       Nom		 VARCHAR2 (15)		NOT NULL,
       Code_postal_U 	 VARCHAR2 (7) 		NOT NULL,
       		CONSTRAINT 	  chk_Code_P_U
         	CHECK (REGEXP_LIKE(
			Code_postal_U ,'[A-z]{1}[0-9]{1}[A-z]{1}[0-9]{1}[A-z]{1}[0-9]{1}','i')),
       Email		 VARCHAR2 (320)	        NOT NULL,
       		CONSTRAINT 	  chk_Email
		CHECK (REGEXP_LIKE(Email,'[A-z]*(.[A-z]*)?@[A-z]*(.[a-z]{2,3})?.[a-z]{2,3}','i')),	
       Hash_pw	      	 VARCHAR2 (16)          NOT NULL, 
       Path_photo_U  	 VARCHAR2 (100),
       Tel 		 VARCHAR2 (15),
                CONSTRAINT 	  chk_Tel
		CHECK (REGEXP_LIKE(Tel ,'[0-9]{3}-[0-9]{3}-[0-9]{4}','i')));

--------------------------------------------------------------------------------
CREATE TABLE TYPE_OBJ(
       Nom_Type		VARCHAR2 (5)		NOT NULL	PRIMARY KEY);

--------------------------------------------------------------------------------
CREATE TABLE OBJET (
       Id_O		NUMBER (5)		NOT NULL	PRIMARY KEY, 	
       Id_Acheteur	NUMBER(5)    		NOT NULL,
       		CONSTRAINT	objet_fk1
		FOREIGN KEY 	(Id_Acheteur)	REFERENCES	UTILISATEUR(Id_U),
       Path_photo_O	VARCHAR2 (100),	
       Id_Annonceur	NUMBER(5)		NOT NULL,
       		CONSTRAINT	objet_fk2
		FOREIGN KEY	(Id_Annonceur)	REFERENCES	UTILISATEUR(Id_U),	
       Nom_O		VARCHAR2 (15)   	NOT NULL,
       Code_postal_O 	VARCHAR2 (7)      	NOT NULL,
       		CONSTRAINT 	chk_Code_P_O
		CHECK (REGEXP_LIKE(
			Code_postal_O ,'[A-z]{1}[0-9]{1}[A-z]{1}[0-9]{1}[A-z]{1}[0-9]{1}')),
       Etat		VARCHAR2 (5)   		NOT NULL,
       		CONSTRAINT	chk_Etat
		CHECK (REGEXP_LIKE(Etat, '(Neuf|Usage)')),
       Prix	      	NUMBER(10,2)		NOT NULL, 	
       Nom_Type		VARCHAR2(15)		NOT NULL,
       		CONSTRAINT	objet_fk3
		FOREIGN KEY (Nom_Type)		REFERENCES	TYPE_OBJ(Nom_Type));

--------------------------------------------------------------------------------
CREATE TABLE PATH_PHOTO_O (
       Id_Photo		NUMBER (5)		NOT NULL	PRIMARY KEY,
       		CONSTRAINT	 path_p_fk
		FOREIGN KEY	 (Id_Photo)	REFERENCES	OBJET(Id_O), 
       Path_img_O  	VARCHAR2(100));

--------------------------------------------------------------------------------
CREATE TABLE MESSAGERIE (
       Id_Destinataire	NUMBER(5)		NOT NULL,
       		CONSTRAINT	mess_fk1
		FOREIGN KEY 	(Id_Destinataire)	REFERENCES	UTILISATEUR(Id_U), 
       Id_Expediteur	NUMBER(5)    		NOT NULL,
       		CONSTRAINT	mess_fk2
		FOREIGN KEY	(Id_Expediteur)		REFERENCES	UTILISATEUR(Id_U), 	
       Date_heure_mess	DATE			NOT NULL, 
       Contenu_M	VARCHAR2(1000)		NOT NULL,
       		PRIMARY KEY (
			Id_Destinataire,
			Id_Expediteur,
			Date_heure_mess));

--------------------------------------------------------------------------------
CREATE TABLE ANNONCE (
	Id_Anc			NUMBER(5)		NOT NULL, 
	Description		VARCHAR2(1000)		NOT NULL,
	Prix_Vente		NUMBER(10,2)		NOT NULL,
	Date_heure_affichage	DATE			NOT NULL,	
	Duree			VARCHAR2(3)		NOT NULL,
			CONSTRAINT	chk_Duree
			CHECK (REGEXP_LIKE(Duree ,'[0-9]{2}Mois-[0-9]{1}Jours')),
	Date_heure_vente	DATE			NOT NULL,
			PRIMARY KEY (
			Id_Anc,
			Date_heure_affichage,
			Date_heure_vente));

--------------------------------------------------------------------------------
CREATE TABLE INTERESSE_A (
	Nom_Type	VARCHAR2 (5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT	 nt_fk1
		FOREIGN KEY	 (Nom_Type)		REFERENCES	TYPE_OBJ(Nom_Type),
	Id_I		NUMBER(5)    			NOT NULL,
		CONSTRAINT	 nt_fk2
		FOREIGN KEY 	 (Id_I)			REFERENCES	UTILISATEUR(Id_U));

--------------------------------------------------------------------------------
CREATE TABLE MARQUE (
	Nom_Marque	VARCHAR2 (15)			NOT NULL,	
	Nom_Type	VARCHAR2 (5) 			NOT NULL,
		CONSTRAINT	 marque_fk
		FOREIGN KEY 	 (Nom_Type)		REFERENCES	TYPE_OBJ(Nom_Type),
		PRIMARY KEY (
			Nom_Marque,
			Nom_Type));

--------------------------------------------------------------------------------
CREATE TABLE TELEPHONE (
	Id_Tel		NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT 	tel_fk
		FOREIGN KEY 	(Id_Tel)		REFERENCES	OBJET(Id_O), 
	Color		VARCHAR2(7)			NOT NULL);

--------------------------------------------------------------------------------
CREATE TABLE PIECE_MECANIQUE (
	Id_PMec		NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT	pm_fk
		FOREIGN KEY 	(Id_PMec)		REFERENCES	OBJET(Id_O));

--------------------------------------------------------------------------------
CREATE TABLE JEUX (
	Id_J			NUMBER(5)		NOT NULL	PRIMARY KEY,
		CONSTRAINT		jeux_fk
		FOREIGN KEY 		(Id_J)		REFERENCES	OBJET(Id_O), 
	Categorie_age		NUMBER(2),
	Nb_joueurs		NUMBER(2));

--------------------------------------------------------------------------------
CREATE TABLE AUTOMOBILE (
	Id_Auto		NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT 	Automobile_fk
		FOREIGN KEY 	(Id_Auto)		REFERENCES	OBJET(Id_O) , 
	Annee		NUMBER(4)			NOT NULL,
	Kilometre	NUMBER(6)			NOT NULL,
	Nb_places	NUMBER(1)			NOT NULL,
	Color		VARCHAR2(10)			NOT NULL);

--------------------------------------------------------------------------------
CREATE TABLE MUSIQUE (
	Id_Msq		NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT 	Musique_fk
		FOREIGN KEY 	(Id_Msq)		REFERENCES	OBJET(Id_O), 		
	Sorte		VARCHAR2(10)			NOT NULL,
	Groupe		VARCHAR2(15),
	Type_musique	VARCHAR2(10));

--------------------------------------------------------------------------------
CREATE TABLE MEUBLE (
	Id_Mub		NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT	Meuble_fk
		FOREIGN KEY 	(Id_Mub)		REFERENCES	OBJET(Id_O), 		
	Color		VARCHAR2(10)	NOT NULL,
	Dimension	VARCHAR2(15),	
	Materiau	VARCHAR2(10));

--------------------------------------------------------------------------------
CREATE TABLE MAISON (
	Id_Msn			NUMBER(5)		NOT NULL	PRIMARY KEY,
		CONSTRAINT		Maison_fk
		FOREIGN KEY 		(Id_Msn)	REFERENCES	OBJET(Id_O), 	
	Annee_Construction	NUMBER(4)		NOT NULL,
	Nb_Chambres		NUMBER(2)		NOT NULL,
	Garage 			VARCHAR2(3)		NOT NULL,
		CONSTRAINT 		chk_Garage
		CHECK (REGEXP_LIKE(Garage, '(OUI|NON)')),	
	Cours_arriere 		VARCHAR2(3)		NOT NULL,
		CONSTRAINT 		chk_Cours_arriere
		CHECK (REGEXP_LIKE(Cours_arriere,'(OUI|NON)')));

--------------------------------------------------------------------------------
CREATE TABLE NOURRITURE (
	Id_N			NUMBER(5)		NOT NULL	PRIMARY KEY,
		CONSTRAINT 		Nourriture_fk
		FOREIGN KEY 		(Id_N)		REFERENCES	OBJET(Id_O), 
	Date_peremption		DATE);

--------------------------------------------------------------------------------
CREATE TABLE VETEMENT (
	Id_Vet			NUMBER(5)		NOT NULL	PRIMARY KEY,   
		CONSTRAINT 		Vet_fk
		FOREIGN KEY		(Id_Vet)	REFERENCES	OBJET(Id_O), 
	Taille_lettre		VARCHAR2(5),
		CONSTRAINT 		chk_Taille_lettre
		CHECK (REGEXP_LIKE (Taille_lettre, '(XXP|XP|P|M|L|XL|XXL|XXXL)')),
	Taille_chiffre		NUMBER(3),			
	Sexe			VARCHAR2(7)		NOT NULL,
		CONSTRAINT		chk_Sexe
		CHECK (REGEXP_LIKE(Sexe, '(HOMME|FEMME|UNISEXE)')),
	Categorie_age		VARCHAR2(10),
		CONSTRAINT		chk_Categorie_age
		CHECK (REGEXP_LIKE(Categorie_age,'(TOUT PETIT|ENFANT|ADO|ADULTE)')),
	Materiau		VARCHAR2(10),
		CONSTRAINT		chk_Materiau_Vet
		CHECK (REGEXP_LIKE(Materiau,'(Acrylique|
					      Angora|
					      Cachemire|
					      Coton|
					      Cuir|
					      Daim|
					      Denim|
					      Faux cuir|
					      Fourrure|
					      Gore-tex|
					      Jersey|
					      Laine|
					      Lin|
					      Lycra|
					      Nylon|
					      Polaire|
					      Polyamide|
					      Polyester|
					      Soie)')));
					      
--------------------------------------------------------------------------------
CREATE TABLE ELECTROMENAGER (
	Id_Elcmngr	NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT 	Electromenager_fk
		FOREIGN KEY 	(Id_Elcmngr)		REFERENCES	OBJET(Id_O));

--------------------------------------------------------------------------------
CREATE TABLE APPAREIL_ELECTRONIQUE (
	Id_Elc		NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT	App_elec_fk
		FOREIGN KEY 	(Id_Elc)		REFERENCES	OBJET(Id_O));

--------------------------------------------------------------------------------
CREATE TABLE BIJOUX (
	Id_Bij		NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT 	Bij_fk
		FOREIGN KEY 	(Id_Bij)		REFERENCES	OBJET(Id_O), 
	Materiau	VARCHAR2(10)			NOT NULL,				
		CONSTRAINT 	Chk_Materiau_Bij
		CHECK (REGEXP_LIKE(Materiau, '(Or|
					       Argent|
					       Cuivre|
				 	       Bronze|
				 	       Titane|
				 	       Laiton|
				 	       Zinc|
					       Palladium|
					       Platine|
					       Rhodium)')));

--------------------------------------------------------------------------------
CREATE TABLE ART (
	Id_Art	 	NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT	Art_fk
		FOREIGN KEY 	(Id_Art)		REFERENCES	OBJET(Id_O),  
	Dimension	VARCHAR2(15));
	
--------------------------------------------------------------------------------
CREATE TABLE NOM_ARTISTE (
	Id_NomArt	NUMBER(5)			NOT NULL,
		CONSTRAINT 	Nom_Artiste_fk
		FOREIGN KEY	(Id_NomArt)		REFERENCES	ART(Id_Art), 
	Nom_Art	        VARCHAR2 (15)   		NOT NULL,
		PRIMARY KEY (
			    Id_NomArt,
			    Nom_Art));

--------------------------------------------------------------------------------
CREATE TABLE LIVRE (
	Id_Livre	NUMBER(5)			NOT NULL	PRIMARY KEY,
		CONSTRAINT	Livre_fk
		FOREIGN KEY 	(Id_Livre)		REFERENCES	OBJET(Id_O),  
	Genre		VARCHAR2(30),
	Nb_Pages	NUMBER(5),
	Editeur		VARCHAR2(50),
	Titre		VARCHAR2(50)			NOT NULL);
	
--------------------------------------------------------------------------------
CREATE TABLE AUTEUR (
       	Id_Auteur	NUMBER(5)			NOT NULL,
		CONSTRAINT 	Auteur_fk
		FOREIGN KEY 	(Id_Auteur)		REFERENCES	LIVRE(Id_Livre), 
	Nom_Auteur	VARCHAR2 (15)   		NOT NULL,
		PRIMARY KEY (
			    Id_Auteur,
			    Nom_Auteur));
