faction 'player'
faction 'scourge'
faction 'chaos'
faction 'orc'
faction 'wild life'

-- player is hostile towards wild life but wild life still is neutral towards player
factionRelation('player',  HOSTILE, 'orc')
factionRelation('player',  HOSTILE, 'wild life')
factionRelation('player',  HOSTILE, 'scourge')
factionRelation('player',  HOSTILE, 'chaos')

factionRelation('scourge', HOSTILE, 'orc')
factionRelation('scourge', HOSTILE, 'wild life')
factionRelation('scourge', HOSTILE, 'player')
factionRelation('scourge', HOSTILE, 'chaos')

factionRelation('chaos', HOSTILE, 'orc')
factionRelation('chaos', HOSTILE, 'wild life')
factionRelation('chaos', HOSTILE, 'player')
factionRelation('chaos', HOSTILE, 'scourge')

factionRelation('orc', HOSTILE, 'wild life')
factionRelation('orc', HOSTILE, 'chaos')
factionRelation('orc', HOSTILE, 'player')
factionRelation('orc', HOSTILE, 'scourge')