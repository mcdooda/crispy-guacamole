faction 'player'
faction 'scourge'
faction 'wild life'

-- player is hostile towards wild life but wild life still is neutral towards player
factionRelation('player',  HOSTILE, 'wild life')
factionRelation('player',  HOSTILE, 'scourge')
factionRelation('scourge', HOSTILE, 'wild life')