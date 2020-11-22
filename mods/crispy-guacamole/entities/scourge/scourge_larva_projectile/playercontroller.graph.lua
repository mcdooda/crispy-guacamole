return {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      1,
      2,
      15,
      1,
    },
    {
      1,
      3,
      10,
      1,
    },
    {
      1,
      3,
      14,
      1,
    },
    {
      1,
      3,
      15,
      2,
    },
    {
      4,
      1,
      9,
      1,
    },
    {
      5,
      1,
      15,
      3,
    },
    {
      7,
      1,
      8,
      1,
    },
    {
      7,
      2,
      3,
      1,
    },
    {
      9,
      1,
      3,
      3,
    },
    {
      9,
      1,
      13,
      2,
    },
    {
      10,
      1,
      12,
      1,
    },
    {
      11,
      1,
      13,
      1,
    },
    {
      12,
      1,
      7,
      2,
    },
    {
      13,
      1,
      8,
      3,
    },
    {
      14,
      1,
      8,
      2,
    },
    {
      14,
      1,
      3,
      2,
    },
    {
      15,
      1,
      7,
      1,
    },
    {
      15,
      3,
      9,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      name = 'components/playercontrollercomponent',
    },
    {
      loadArguments = {
        {
          5,
          'Component',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      name = 'entity/setspriterotation',
    },
    {
      name = 'game/gametime',
    },
    {
      loadArguments = {
        'rotation_speed',
      },
      name = 'string',
    },
    {
      loadArguments = {
        'rotation_speed',
      },
      name = 'string',
    },
    {
      name = 'script/condition',
    },
    {
      name = 'entity/setspriterotation',
    },
    {
      name = 'math/multiply',
    },
    {
      name = 'entity/getheading',
    },
    {
      loadArguments = {
        -1,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'data/compounds/script/Is Looking Left',
      },
      name = 'compound',
    },
    {
      name = 'math/multiply',
    },
    {
      name = 'reroute',
    },
    {
      name = 'entity/getextradatafield',
    },
  },
}