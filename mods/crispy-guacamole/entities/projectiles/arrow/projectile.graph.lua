return {
  links = {
    {
      1,
      1,
      9,
      1,
    },
    {
      2,
      1,
      13,
      2,
    },
    {
      2,
      1,
      11,
      1,
    },
    {
      2,
      1,
      20,
      1,
    },
    {
      2,
      1,
      27,
      1,
    },
    {
      3,
      1,
      24,
      3,
    },
    {
      4,
      1,
      23,
      3,
    },
    {
      6,
      1,
      1,
      3,
    },
    {
      8,
      1,
      23,
      2,
    },
    {
      10,
      1,
      24,
      2,
    },
    {
      10,
      1,
      2,
      1,
    },
    {
      11,
      1,
      9,
      2,
    },
    {
      11,
      1,
      12,
      1,
    },
    {
      12,
      1,
      22,
      2,
    },
    {
      13,
      1,
      23,
      1,
    },
    {
      14,
      1,
      22,
      1,
    },
    {
      15,
      1,
      21,
      2,
    },
    {
      16,
      1,
      17,
      1,
    },
    {
      17,
      1,
      1,
      2,
    },
    {
      18,
      1,
      1,
      4,
    },
    {
      19,
      1,
      13,
      3,
    },
    {
      20,
      1,
      1,
      5,
    },
    {
      21,
      1,
      1,
      1,
    },
    {
      21,
      2,
      24,
      1,
    },
    {
      23,
      4,
      14,
      1,
    },
    {
      23,
      5,
      26,
      1,
    },
    {
      24,
      1,
      13,
      1,
    },
    {
      25,
      1,
      28,
      1,
    },
    {
      25,
      2,
      21,
      1,
    },
    {
      25,
      3,
      10,
      1,
    },
    {
      25,
      4,
      15,
      1,
    },
    {
      25,
      4,
      16,
      1,
    },
    {
      26,
      1,
      14,
      2,
    },
    {
      27,
      1,
      23,
      4,
    },
    {
      29,
      1,
      25,
      3,
    },
    {
      30,
      1,
      25,
      1,
    },
    {
      31,
      1,
      25,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      name = 'entity/dealdamage',
    },
    {
      name = 'reroute',
    },
    {
      loadArguments = {
        'projectile',
      },
      name = 'string',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'fx_explosion',
      },
      name = 'string',
    },
    {
      loadArguments = {
        3,
      },
      name = 'number',
    },
    {
      name = 'entity/despawnentity',
    },
    {
      name = 'reroute',
    },
    {
      name = 'reroute',
    },
    {
      name = 'reroute',
    },
    {
      name = 'entity/disablecomponent',
    },
    {
      name = 'script/condition',
    },
    {
      name = 'entity/isvalidentity',
    },
    {
      name = 'reroute',
    },
    {
      name = 'reroute',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'collision',
      },
      name = 'string',
    },
    {
      name = 'entity/getinstigator',
    },
    {
      name = 'script/condition',
    },
    {
      name = 'entity/despawnentity',
    },
    {
      name = 'game/timer',
    },
    {
      name = 'entity/disablecomponent',
    },
    {
      name = 'components/projectilecomponent',
    },
    {
      name = 'entity/isvalidentity',
    },
    {
      name = 'reroute',
    },
    {
      loadArguments = {
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        30,
      },
      name = 'number',
    },
    {
      loadArguments = {
        12,
      },
      name = 'number',
    },
  },
}