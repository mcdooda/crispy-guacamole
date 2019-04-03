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
      12,
      1,
    },
    {
      1,
      3,
      4,
      1,
    },
    {
      1,
      3,
      5,
      1,
    },
    {
      1,
      3,
      9,
      2,
    },
    {
      1,
      3,
      11,
      2,
    },
    {
      1,
      3,
      16,
      2,
    },
    {
      1,
      3,
      38,
      2,
    },
    {
      1,
      3,
      22,
      2,
    },
    {
      1,
      3,
      21,
      2,
    },
    {
      1,
      3,
      12,
      2,
    },
    {
      1,
      3,
      37,
      2,
    },
    {
      1,
      3,
      18,
      1,
    },
    {
      1,
      3,
      29,
      2,
    },
    {
      1,
      3,
      39,
      2,
    },
    {
      1,
      3,
      40,
      1,
    },
    {
      3,
      1,
      11,
      1,
    },
    {
      3,
      6,
      17,
      1,
    },
    {
      4,
      1,
      8,
      1,
    },
    {
      5,
      1,
      6,
      1,
    },
    {
      6,
      1,
      7,
      1,
    },
    {
      6,
      2,
      7,
      2,
    },
    {
      7,
      1,
      8,
      2,
    },
    {
      8,
      1,
      9,
      3,
    },
    {
      9,
      1,
      28,
      1,
    },
    {
      9,
      1,
      14,
      1,
    },
    {
      10,
      1,
      9,
      4,
    },
    {
      11,
      1,
      9,
      1,
    },
    {
      12,
      1,
      3,
      1,
    },
    {
      13,
      1,
      12,
      3,
    },
    {
      14,
      1,
      38,
      1,
    },
    {
      15,
      1,
      25,
      2,
    },
    {
      16,
      1,
      22,
      1,
    },
    {
      17,
      1,
      16,
      1,
    },
    {
      18,
      1,
      32,
      1,
    },
    {
      18,
      1,
      33,
      1,
    },
    {
      19,
      1,
      38,
      3,
    },
    {
      20,
      1,
      37,
      3,
    },
    {
      21,
      1,
      25,
      1,
    },
    {
      23,
      1,
      16,
      3,
    },
    {
      24,
      1,
      22,
      4,
    },
    {
      25,
      1,
      17,
      2,
    },
    {
      26,
      1,
      22,
      3,
    },
    {
      27,
      1,
      34,
      2,
    },
    {
      28,
      1,
      37,
      1,
    },
    {
      29,
      1,
      30,
      1,
    },
    {
      30,
      1,
      14,
      2,
    },
    {
      31,
      1,
      29,
      1,
    },
    {
      32,
      1,
      20,
      2,
    },
    {
      32,
      2,
      20,
      1,
    },
    {
      33,
      1,
      34,
      1,
    },
    {
      34,
      1,
      28,
      2,
    },
    {
      35,
      1,
      30,
      2,
    },
    {
      36,
      1,
      21,
      1,
    },
    {
      38,
      1,
      39,
      1,
    },
    {
      40,
      1,
      41,
      1,
    },
    {
      41,
      1,
      39,
      3,
    },
    {
      42,
      1,
      41,
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
      loadArguments = {
        7,
      },
      name = 'script/impulseall',
    },
    {
      name = 'controller/leftstickdirection',
    },
    {
      name = 'entity/getposition',
    },
    {
      name = 'math/vector3/vector3split',
    },
    {
      name = 'vector2',
    },
    {
      name = 'math/vector2/vector2add',
    },
    {
      name = 'entity/moveto',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      name = 'entity/clearpath',
    },
    {
      name = 'entity/setstrafing',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
    {
      name = 'script/condition',
    },
    {
      loadArguments = {
        0.5,
      },
      name = 'number',
    },
    {
      name = 'flow/cooldown',
    },
    {
      name = 'script/condition',
    },
    {
      name = 'controller/rightstickdirection',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        'atan(y,x)',
      },
      name = 'math/formula',
    },
    {
      name = 'controller/axisvalue',
    },
    {
      loadArguments = {
        'mods/gdf/compounds/script/Fire Missile',
      },
      name = 'compound',
    },
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      loadArguments = {
        0,
      },
      name = 'number',
    },
    {
      name = 'math/greaterthan',
    },
    {
      loadArguments = {
        'missile3',
      },
      name = 'string',
    },
    {
      loadArguments = {
        0.5,
      },
      name = 'number',
    },
    {
      name = 'script/condition',
    },
    {
      name = 'controller/axisvalue',
    },
    {
      name = 'math/greaterthan',
    },
    {
      loadArguments = {
        'TRIGGERLEFT',
      },
      name = 'string',
    },
    {
      name = 'math/vector2/vector2split',
    },
    {
      name = 'math/vector2/vector2length',
    },
    {
      name = 'math/greaterthan',
    },
    {
      loadArguments = {
        0.5,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'TRIGGERRIGHT',
      },
      name = 'string',
    },
    {
      name = 'entity/setheading',
    },
    {
      name = 'entity/jump',
    },
    {
      name = 'entity/setmidairacceleration',
    },
    {
      name = 'entity/getforward',
    },
    {
      name = 'math/vector3/vector3numbermultiply',
    },
    {
      loadArguments = {
        10,
      },
      name = 'number',
    },
  },
}