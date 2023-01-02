_**Lucie Houdeyer et moi avons réalisé un server IRC basé sur la RFC en CPP, nous avons utilisé en client par défaut le IRSSI**_

## Sujet

Executable: ./ircserver port password

- Le serveur IRC doit pouvoir recevoir/manipuler plusieurs clients en meme temps sans crash.
- Interdiction de fork. fd non-bloques.
- 1 poll() autorise pour read, write, listen... Poll() obligatoire.
- Protocol de communication : TCP/IP.  
- Features a implenter :
	1. On doit pouvoir s'authentifier, set un nickname, a username, rejoindre un channel,
	envoyer et recevoir des messages prives.
	2. Tous les messages envoyes a un channel d`un client doivent etre envoyes aux clients
	qui ont rejoint le channel.
	(bref les users doivent recevoir/pouvoir lire les messages envoyes par un autre user).
	3. On doit avoir des operateurs (modo quoi) et des users lambdas.
	4. On doit implenter les commandes specifiques au modo.

## Liens utiles 
Pour la grammaire/syntaxe :
	http://abcdrfc.free.fr/rfc-vf/rfc5234.html

RFC 2810, RFC, 2811, RFC 2812, RFC 2813 :

https://www.rfc-editor.org/rfc/rfc2810.html

https://www.rfc-editor.org/rfc/rfc2811.html
	
https://www.rfc-editor.org/rfc/rfc2812.html
	
https://www.rfc-editor.org/rfc/rfc2813.html

Pour les modes : 
	https://www.alien.net.au/irc/chanmodes.html

## Channel Management Rules

- Namespace :

	Nom/titre d`un channel est une string conmmencant par `&`, `#`, `+` or `!` (prefixe). 
	Longueur maximum : 50 caracteres. Ne doit pas contenir de ` `, `,` ou CTRL+G.
- Privileged channel members : 

	Peuvent :  
	INVITE	- peuvent inviter un user en mode invitation-seulement (mode +i)
	KICK	- bon, c`est assez clair.
	MODE	- permet de changer le mode d`un channel. 
	PRIVMSG	- envoie d`un message dans un channel (mode +n, +m, +v)
	TOPIC	- change le topic/sujet d`un channel (mode +t)
- Channel Operators :

	Les operateurs d`un channel (sauf apres l`abolition de la propriete privee).
	Identifiables par le `@` devant leur nickname.
	Si un channel commence par le prefixe `+`, le chan ne supporte pas les modes
	et ne peut pas avoir d`operateurs.
	Peut connecter et deconnecter des serveurs, virer de force (BOUHHHH) un user. 
- Channel creator : 

	"owner" du channel.
	Un user qui cree un chan avec prefixe `!` est identifie comme le.a createur.trice 
	du channel. Cet user a aussi les droits operateurs. 
	Il peut appliquer certains modes sur un chan contrairement aux operateurs. 
	Bref createur > operateur.
- Standard channel : 

	Channel cree quand le premier user le rejoint et cesse d`exister 
	quand le dernier user le quitte (est-ce que c`est comme ca pour une conv messenger ?)
	La personne qui cree un channel devient automatiquement un channel operateur
	quand le channel commence par le prefixe `+`
- Safe channel : 

	Un channel safe n`est pas automatiquement cree quand un user le rejoint. 
	Pour le creer il faut faire une requete en tapant la commande JOIN.
	Le prefixe d`un channel safe commence par `!`
	Le process de creation est totalement controle. Le user choisi un parti du nom ("short name").
	Puis automatiquement un identifiant (5 caracteres) est donne. 
	Le nom final est un mix du short name et de cet identifiant. 
	C`est donc unique et securise. 
	La personne qui cree le channel devient son createur. Et elle est la seule a l`etre,
	meme si plus personne n`est dessus et qu`un utilisateur rejoint le channel.
- Channel modes :

	* O - donne le statut de channel createur (can be used in "safe channels" and SHALL NOT be manipulated by users);
	* o - donne les privileges d`un channel operateur;
	v - donne le privilege de la voix, avec ce privilege les personnes peuvent parler sur un chan modere;
	a - anonyme channel, anonymous!anonymous@anonymous;
	* i - mode invitation-seulement, possibilite de rejoindre le channel uniquement sur invitation;
	m - channel modere, seul les personnes avec le flag `m` ou moderateur/operateur peuvent parler;
	n - ce flag affecte seulement les users, seuls les membres du channel peuvent parler;
	q - serveur uniquement, les activites du serveur (membre qui rejoint, changement de nicname...) 
	ne sont pas envoyes sur le serveur;
	p - rend un channel prive, impossibilite de faire un WHOIS sans etre dans le channel;
	s - pareil que le mode+p mais en plus restrictif, pas de TOPIC, LIST, NAME cmd;
	r - seul le createur peut set ce mode (franchement j`ai pas compris et je pense qu`il n`y a pas besoin de le faire);
	t - restreint les droits des operateurs (je crois a verifier ???);
	k - set/remove the channel key (password);
	l - set/remove the user limit to channel;
	b - set/remove ban mask to keep users out;
	e - set/remove an exception mask to override a ban mask;
	I - set/remove an invitation mask to automatically override the invite-only flag;
	Unless mentioned otherwise below, all these modes can be manipulated
	by "channel operators" by using the MODE command defined in "IRC client Protocol"
- Users : 

	possede un nickname unique compose de maximum 9 caracteres 
	
Le prefix, la commande et les parametres sont separes par un espace (0x20).

## Commandes Implémentées :

### PASS  
	Param : < password >
	
	Cette commande permet de set une connection avec mdp.
		
### NICK  
	Param : < nickname >
	Permet d`attribuer ou changer de nickname.

### USER  
	Param : < user > < mode > < unused > < realname >
	
	La commande USER est utilise lors de la connection pour specifier le username, hostname et realname d`un nouveau user.
	
	Le param < mode > doit etre un numerique et, bitmask. Si jamais le deuxieme bit est set, le mode+w est active (user receives wallops - j`ai pas compris),
	
	si jamais le bit 3 est setm le mode+i (marks a users as invisible) est active.
	
	Le < realname > peut contenir des espaces.
	
### MODE  
	Parameters: < nickname >  	
		*( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )
		
	a - user is flagged as away.  
	i - marks a users as invisible.  
	w - user receives wallops.  
	r - restricted user connection.  
	o - operator flag.  
	O - local operator flag.  
	s - marks a user for receipt of server notice.  

### OPER  
	Parameters: < name > < password >  
	Un user lambda utilise la cmd OPER pour obtenir les privileges des operateurs.    
	La combinaison < name > < password > est obligatoire afin d'obtenir les droits.    


### QUIT  
	Parameters : [ < Quit Message > ]. 
	La session est terminee, envoi d`un court message.  

### SQUIT  
	Params : < server > < comment >.  
	Deconnecte un serveur.   

### JOIN  
	Params : ( < channel > *( "," < channel > ) [ < key > *( "," < key > ) ] )  
				/ "0"  
	La cmd JOIN permet de rejoindre un channel.  
	Une fois qu`un user rejoint un channel, il recoit les infos concernant ce-dernier :   
		toutes les commandes acceptees dans ce chan, JOIN, MODE, KICK, PART, QUIT, PRIVMSG/NOTICE.  
	Si jamais le user parvient a rejoindre le channel, il recoit un msg indiquant qu`il a bien rejoint le channel,  
	recoit le TOPIC du channel (RPL_TOPIC) et la liste des membres du chan (RPL_NAMREPLY), liste dans laquelle il
	est lui-meme inclu.  

### PART   
	Params	:	< channel > *( "," < channel > ) [ < Part Message > ]. 
	Permet de supprimer le user des listes des membres actifs des channels.  
	Si jamais un < part message > est passe en parametres, il remplace le msg par defaut.  

### MODE  
	Parameters: < channel > *( ( "-" / "+" ) *< modes > *< modeparams > ).  
	commande MODE permet de changer le(s) mode(s)/caracteristiques d`un channel.  
	Maximum trois modes par commandes.  

### NAMES   
	Parameters: [ < channel > *( "," < channel > ) [ < target > ] ]  
	La cmd NAMES permet de lister tous les nicknames visibles d`un channel  
	Si jamais aucun channel n`est passe en param, une list des channel et des 
	users est retourne.  

### LIST  
	Parameters: [ < channel > *( "," < channel > ) [ < target > ] ]. 
	Permets de lister les channels et leurs topics.  

