return {
  links = {
    {
      2,
      1,
      6,
      1,
    },
    {
      3,
      1,
      2,
      1,
    },
    {
      4,
      1,
      6,
      2,
    },
    {
      5,
      1,
      4,
      1,
    },
    {
      6,
      1,
      1,
      1,
    },
  },
  nodeType = 'sound',
  nodes = {
    {
      name = 'sound/soundoutput',
    },
    {
      name = 'sound/sample',
    },
    {
      loadArguments = {
        'human/male1_death1.ogg',
      },
      name = 'string',
    },
    {
      name = 'sound/sample',
    },
    {
      loadArguments = {
        'human/male1_death2.ogg',
      },
      name = 'string',
    },
    {
      loadArguments = {
        3,
      },
      name = 'sound/random',
    },
  },
}