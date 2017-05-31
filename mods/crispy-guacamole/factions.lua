faction 'player'
faction 'scourge'
faction 'chaos'
faction 'wild life'

-- player is hostile towards wild life but wild life still is neutral towards player
factionRelation('player',  HOSTILE, 'wild life')
factionRelation('player',  HOSTILE, 'scourge')
factionRelation('player',  HOSTILE, 'chaos')

factionRelation('scourge', HOSTILE, 'wild life')
factionRelation('scourge', HOSTILE, 'player')
factionRelation('scourge', HOSTILE, 'chaos')

factionRelation('chaos', HOSTILE, 'wild life')
factionRelation('chaos', HOSTILE, 'player')
factionRelation('chaos', HOSTILE, 'scourge')