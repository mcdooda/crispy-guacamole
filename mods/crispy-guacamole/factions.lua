faction 'human'
faction 'scourge'
faction 'chaos'
faction 'orc'
faction 'wild life'

-- human is hostile towards wild life but wild life still is neutral towards human
factionRelation('human',  HOSTILE, 'orc')
factionRelation('human',  HOSTILE, 'wild life')
factionRelation('human',  HOSTILE, 'scourge')
factionRelation('human',  HOSTILE, 'chaos')

factionRelation('scourge', HOSTILE, 'orc')
factionRelation('scourge', HOSTILE, 'wild life')
factionRelation('scourge', HOSTILE, 'human')
factionRelation('scourge', HOSTILE, 'chaos')

factionRelation('chaos', HOSTILE, 'orc')
factionRelation('chaos', HOSTILE, 'wild life')
factionRelation('chaos', HOSTILE, 'human')
factionRelation('chaos', HOSTILE, 'scourge')

factionRelation('orc', HOSTILE, 'wild life')
factionRelation('orc', HOSTILE, 'chaos')
factionRelation('orc', HOSTILE, 'human')
factionRelation('orc', HOSTILE, 'scourge')